#include "engine.h"
#include "framebuffer.h"

Engine::Engine(const std::string &title, int width, int height)
    : window(title, width, height),
      camera(),
      scene(camera),
      inputSystem(&camera, &window),
      framebuffer(800, 600),
      editor(window, scene, scriptSystem, framebuffer) {}

void Engine::run()
{
  scriptSystem.addScript("/Users/adriankiezik/3dengine/src/engine.cpp");
  std::vector<std::pair<std::string, std::string>> texturePaths = {
      {"texture_diffuse", "../resources/gitignored/table/DefaultMaterial_BaseColor.png"}};
  Model model("../resources/gitignored/table/Table.obj", texturePaths, "../shaders/vertex_shader.glsl", "../shaders/fragment_shader.glsl");

  scene.addModel(model);

  camera.setAspectRatio(static_cast<float>(800 / 600));

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
  framebuffer.bind();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  scene.update();
  framebuffer.unbind();
}

void Engine::postUpdate()
{
  window.postUpdate();
}
