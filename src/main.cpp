#include <iostream>
#include "window.h"
#include "editor.h"
#include "input_handler.h"
#include "camera.h"
#include "model.h"
#include "scene.h"

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

  InputHandler inputHandler(&camera, window);
  inputHandler.init();

  // Model tableModel("/Users/adriankiezik/3dengine/resources/table/table.obj",
  //                  "../shaders/vertex_shader.glsl",
  //                  "../shaders/fragment_shader.glsl");

  // scene.addModel(tableModel);

  while (!window.shouldClose())
  {
    inputHandler.update();
    window.update();
    scene.draw();
    editor.update();
    window.postUpdate();
  }

  return 0;
}
