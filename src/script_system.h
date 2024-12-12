#ifndef SCRIPT_SYSTEM_H
#define SCRIPT_SYSTEM_H

#include <sol/sol.hpp>

struct Script
{
  std::string name;
  std::string path;
  std::string content;
};

class ScriptSystem
{
public:
  ScriptSystem();
  void init();
  void update();

  void addScript(const std::string &path);

  std::vector<Script> getScripts() const;
  std::vector<std::string> getCapturedMessages() const;

private:
  sol::state lua;
  std::vector<Script> scripts;
  std::vector<std::string> capturedMessages;

  std::string loadScriptFromPath(const std::string &path);
  void capturePrint(const std::string &message);
};

#endif
