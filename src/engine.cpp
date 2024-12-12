#include "engine.h"

Engine::Engine(const std::string &title, int width, int height)
    : window(title, width, height),
      camera(),
      scene(camera, window.getFramebuffer()),
      inputSystem(&camera, &window),
      editor(window, scene, camera, scriptSystem, window.getFramebuffer(), consoleBuffer) {}

void Engine::run()
{
  std::vector<std::pair<std::string, std::string>> texturePaths = {
      {"texture_diffuse", "../resources/gitignored/table/DefaultMaterial_BaseColor.png"}};
  Model model("../resources/gitignored/table/Table.obj", texturePaths, "../shaders/universal_vertex_shader.glsl", "../shaders/universal_fragment_shader.glsl");
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
  editor.update();
  scene.update();
}

void Engine::postUpdate()
{
  window.postUpdate();
}
