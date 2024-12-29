#ifndef SCRIPT_SYSTEM_H
#define SCRIPT_SYSTEM_H

#include <sol/sol.hpp>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <nlohmann/json.hpp>
#include "script.h"
#include "scene.h"

class ScriptSystem
{
public:
  ScriptSystem(Scene &scene);
  void init();
  void update();

  std::vector<std::string> getCapturedMessages() const;

private:
  sol::state lua;
  Scene &scene;
  std::vector<std::string> capturedMessages;
  std::set<std::string> initializedScripts;
  std::map<std::string, sol::environment> scriptEnvironments;  // Store environments for each script

  void capturePrint(const std::string &message);
  void executeScript(const Script &script, Object &object);
  std::string getScriptKey(const Script &script, const Object &object);
};

#endif
