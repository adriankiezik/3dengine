#include "script_system.h"
#include <iostream>
#include <unordered_set>
#include <string>
#include <unordered_map>

ScriptSystem::ScriptSystem(Scene &scene) : scene(scene)
{
  init();
}

void ScriptSystem::init()
{
  lua.open_libraries(sol::lib::base, sol::lib::math);

  // Capture print output
  lua["print"] = [this](const std::string &message)
  {
    this->capturePrint(message);
  };

  // Register glm::vec3 type and constructor function
  lua["vec3"] = [](float x, float y, float z) -> glm::vec3 {
    return glm::vec3(x, y, z);
  };

  auto vec3Type = lua.new_usertype<glm::vec3>("vec3_type",
    sol::constructors<glm::vec3(float, float, float)>(),
    "x", &glm::vec3::x,
    "y", &glm::vec3::y,
    "z", &glm::vec3::z
  );

  // Register Object API
  auto objectType = lua.new_usertype<Object>("Object",
    "getPosition", &Object::getPosition,
    "setPosition", &Object::setPosition,
    "getRotation", &Object::getRotation,
    "setRotation", &Object::setRotation,
    "getScale", &Object::getScale,
    "setScale", &Object::setScale,
    "getName", &Object::getName,
    "setName", &Object::setName);
}

void ScriptSystem::update()
{
  for (auto &object : scene.getObjects())
  {
    for (const auto &script : object.getScripts())
    {
      executeScript(script, object);
    }
  }
}

void ScriptSystem::executeScript(const Script &script, Object &object)
{
  try
  {
    std::string scriptKey = getScriptKey(script, object);

    // Get or create environment for this script
    sol::environment &env = scriptEnvironments[scriptKey];
    if (!env)
    {
      env = sol::environment(lua, sol::create, lua.globals());
      // Execute the script in its environment
      sol::protected_function_result result = lua.safe_script(script.content, env, &sol::script_pass_on_error);
      if (!result.valid())
      {
        sol::error err = result;
        std::cerr << "Lua compile error: " << err.what() << std::endl;
        return;
      }
    }

    // Make the object available to the script
    env["self"] = &object;

    // Call init() function if it exists and hasn't been initialized yet
    if (initializedScripts.find(scriptKey) == initializedScripts.end())
    {
      if (auto init = env["init"]; init.valid() && init.get_type() == sol::type::function)
      {
        sol::protected_function pf = init;
        sol::protected_function_result result = pf();
        if (!result.valid())
        {
          sol::error err = result;
          std::cerr << "Lua init() error: " << err.what() << std::endl;
        }
        else
        {
          initializedScripts.insert(scriptKey);
        }
      }
    }

    // Call update() function if it exists
    if (auto update = env["update"]; update.valid() && update.get_type() == sol::type::function)
    {
      sol::protected_function pf = update;
      sol::protected_function_result result = pf();
      if (!result.valid())
      {
        sol::error err = result;
        std::cerr << "Lua update() error: " << err.what() << std::endl;
      }
    }
  }
  catch (const std::exception &e)
  {
    std::cerr << "C++ error executing script " << script.name << ": " << e.what() << std::endl;
  }
}

std::string ScriptSystem::getScriptKey(const Script &script, const Object &object)
{
  // Create a unique key combining script name and object ID/name
  return script.name + "_" + object.getName();
}

void ScriptSystem::capturePrint(const std::string &message)
{
  std::cout << message << std::endl;
  capturedMessages.push_back(message);
}

std::vector<std::string> ScriptSystem::getCapturedMessages() const
{
  return capturedMessages;
}