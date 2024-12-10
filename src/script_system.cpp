#include "script_system.h"
#include <fstream>
#include <sstream>
#include <iostream>

ScriptSystem::ScriptSystem()
{
  init();
}

void ScriptSystem::init()
{
  lua.open_libraries(sol::lib::base);
  lua.script(R"(
        print('Hello from Lua!')
    )");
}

void ScriptSystem::update() {}

void ScriptSystem::addScript(const std::string &path)
{
  if (std::find_if(scripts.begin(), scripts.end(), [&path](const Script &s)
                   { return s.path == path; }) != scripts.end())
  {
    return;
  }

  std::string content = loadScriptFromPath(path);

  std::filesystem::path p(path);

  Script newScript{p.filename().string(), path, content};

  scripts.push_back(newScript);
}

std::vector<Script> ScriptSystem::getScripts()
{
  return scripts;
}

std::string ScriptSystem::loadScriptFromPath(const std::string &path)
{
  try
  {
    std::ifstream file(path);
    if (!file.is_open())
    {
      throw std::runtime_error("Could not open script file: " + path);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string script = buffer.str();

    std::cout << "Successfully loaded script from: " << path << std::endl;

    return script;
  }
  catch (const std::exception &e)
  {
    std::cerr << "Error loading script: " << e.what() << std::endl;

    return "";
  }
}
