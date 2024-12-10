#include "script_system.h"

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