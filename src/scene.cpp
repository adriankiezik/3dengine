#include "scene.h"
#include <iostream>
#include "framebuffer.h"

Scene::Scene(
    Camera &camera,
    Framebuffer &framebuffer) : camera(camera), framebuffer(framebuffer),
                                grid(3.0f, "../shaders/vertex_shader.glsl", "../shaders/fragment_shader.glsl") {}

void Scene::addModel(const Model &model)
{
  models.push_back(model);
}

void Scene::update()
{
  framebuffer.bind();

  grid.render(glm::mat4(1.0f), camera.getViewMatrix(), camera.getProjectionMatrix(), camera.getPosition(), 50, glm::vec3(0.5f, 0.5f, 0.5f));

  for (Model &model : models)
  {
    model.Draw(camera.getViewMatrix(), camera.getProjectionMatrix());
  }

  framebuffer.unbind();
}
