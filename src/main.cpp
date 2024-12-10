#include <iostream>
#include "window.h"
#include "editor.h"
#include "input_system.h"
#include "camera.h"
#include "model.h"
#include "scene.h"
#include <sol/sol.hpp>

int main()
{
  Window window("3dengine", 800, 600);

  if (!window.init())
  {
    return -1;
  }

  Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

  Scene scene(window, camera);

  Editor editor(window, scene);

  if (!editor.init())
  {
    return -1;
  }

  InputSystem inputSystem(&camera, window);
  inputSystem.init();

  sol::state lua;
  lua.open_libraries(sol::lib::base);
  lua.script(R"(
        print('Hello from Lua!')
    )");

  while (!window.shouldClose())
  {
    inputSystem.update();
    window.update();
    scene.draw();
    editor.update();
    window.postUpdate();
  }

  return 0;
}
