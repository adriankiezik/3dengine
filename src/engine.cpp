#include "engine.h"

Engine::Engine(const std::string &title, int width, int height)
    : window(title, width, height),
      camera(),
      scene(window, camera),
      inputSystem(&camera, &window),
      editor(window, scene, scriptSystem) {}

void Engine::run()
{
  scriptSystem.addScript("/Users/adriankiezik/3dengine/src/engine.cpp");
  std::vector<std::pair<std::string, std::string>> texturePaths = {
      {"texture_diffuse", "../resources/gitignored/table/DefaultMaterial_BaseColor.png"}};
  Model model("../resources/gitignored/table/Table.obj", texturePaths, "../shaders/vertex_shader.glsl", "../shaders/fragment_shader.glsl");

  scene.addModel(model);

  while (!window.shouldClose())
  {
    update();
    postUpdate();
  }
}

void Engine::update()
{
  window.update();
  inputSystem.update();
  scriptSystem.update();
  scene.update();
  editor.update();
}

void Engine::postUpdate()
{
  window.postUpdate();
}
