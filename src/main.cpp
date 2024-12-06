#include <iostream>
#include "window.h"
#include "editor.h"
#include "input_handler.h"
#include "camera.h"
#include "cube.h"

int main() {
  Window window("3dengine", 800, 600);

  if (!window.initialize()) {
    return -1;
  }

  Editor editor(window);

  if (!editor.initialize()) {
    return -1;
  }

  Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), // Position
                glm::vec3(0.0f, 1.0f, 0.0f), // Up vector
                -90.0f, // Yaw
                0.0f, // Pitch
                2.5f, // Movement speed
                0.1f, // Mouse sensitivity
                45.0f); // Field of View

  InputHandler inputHandler(&camera, window);
  inputHandler.initialize();

  Cube cube(camera);
  cube.initialize();

  while (!window.shouldClose()) {
    inputHandler.update();
    window.update();
    cube.update();
    editor.update();
    window.postUpdate();
  }

  return 0;
}
