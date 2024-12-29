#include "script.h"

Script::Script(const std::string &scriptPath, const std::string &scriptContent)
    : path(scriptPath), content(scriptContent)
{
  extractNameFromPath();
}

Script::Script(const std::string &scriptPath) : path(scriptPath)
{
  std::ifstream file(scriptPath);
  if (!file.is_open())
  {
    throw std::runtime_error("Could not open script file: " + scriptPath);
  }

  std::stringstream buffer;
  buffer << file.rdbuf();
  content = buffer.str();

  extractNameFromPath();
}

void Script::extractNameFromPath()
{
  size_t lastSlash = path.find_last_of("/\\");
  size_t lastDot = path.find_last_of(".");
  if (lastSlash == std::string::npos)
    lastSlash = -1;
  name = path.substr(lastSlash + 1, lastDot - lastSlash - 1);
}