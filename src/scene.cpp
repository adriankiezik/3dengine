#include "scene.h"
#include <iostream>
#include "framebuffer.h"

Scene::Scene(
    Camera &camera,
    Framebuffer &framebuffer) : camera(camera), framebuffer(framebuffer),
                                grid(1.5f, "../shaders/grid_vertex_shader.glsl", "../shaders/grid_fragment_shader.glsl") {}

void Scene::addModel(const Model &model)
{
  models.push_back(model);
}

void Scene::update()
{
  framebuffer.bind();

  grid.render(glm::mat4(1.0f), camera.getViewMatrix(), camera.getProjectionMatrix(), camera.getPosition(), 100, glm::vec3(0.4f, 0.6f, 0.8f));

  for (Model &model : models)
  {
    model.Draw(camera.getViewMatrix(), camera.getProjectionMatrix());
  }

  framebuffer.unbind();
}
