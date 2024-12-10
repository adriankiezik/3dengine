#include "engine.h"

Engine::Engine(const std::string &title, int width, int height)
    : window(title, width, height),
      camera(glm::vec3(0.0f, 0.0f, 3.0f)),
      scene(window, camera),
      inputSystem(&camera, &window),
      editor(window, scene) {}

void Engine::run()
{
  while (!window.shouldClose())
  {
    update();
    postUpdate();
  }
}

void Engine::update()
{
  inputSystem.update();
  window.update();
  scriptSystem.update();
  scene.update();
  editor.update();
}

void Engine::postUpdate()
{
  window.postUpdate();
}
