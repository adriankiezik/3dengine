#include <iostream>
#include "window.h"
#include "editor.h"
#include "input_handler.h"
#include "camera.h"
#include "cube.h"
#include "model.h"

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

  // Cube cube(camera);
  // cube.init();

  Model myModel("/Users/adriankiezik/3dengine/resources/table/table.fbx",
                "/Users/adriankiezik/3dengine/resources/table/");

  Shader shader("../shaders/vertex_shader.glsl", "../shaders/fragment_shader_no_texture.glsl");

  glm::mat4 model = glm::mat4(1.0f);

  while (!window.shouldClose())
  {
    inputHandler.update();
    window.update();
    // cube.update();
    shader.use();
    shader.setMat4("model", model);
    shader.setMat4("view", camera.getViewMatrix());
    shader.setMat4("projection", camera.getProjectionMatrix(window.getWidth() / window.getHeight()));
    myModel.Draw(shader.id);
    editor.update();
    window.postUpdate();
  }

  return 0;
}
