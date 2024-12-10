#ifndef SCRIPT_SYSTEM_H
#define SCRIPT_SYSTEM_H

#include <sol/sol.hpp>

class ScriptSystem
{
public:
  ScriptSystem();
  void init();
  void update();

private:
  sol::state lua;
};

#endif
