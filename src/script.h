#ifndef SCRIPT_H
#define SCRIPT_H

#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>

struct Script
{
  std::string name;
  std::string path;
  std::string content;

private:
  void extractNameFromPath();

public:
  Script(const std::string &scriptPath, const std::string &scriptContent);
  Script(const std::string &scriptPath);
};

#endif
