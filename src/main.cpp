#include <iostream>
#include "window.h"
#include "editor.h"
#include "input_handler.h"
#include "camera.h"
#include "cube.h"

int main()
{
  Window window("3dengine", 800, 600);

  if (!window.init())
  {
    return -1;
  }

  Editor editor(window);

  if (!editor.init())
  {
    return -1;
  }

  Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

  InputHandler inputHandler(&camera, window);
  inputHandler.init();

  Cube cube(camera);
  cube.init();

  while (!window.shouldClose())
  {
    inputHandler.update();
    window.update();
    cube.update();
    editor.update();
    window.postUpdate();
  }

  return 0;
}
