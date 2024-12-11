#include "scene.h"
#include <iostream>

Scene::Scene(Camera &camera) : camera(camera) {}

void Scene::addModel(const Model &model)
{
  models.push_back(model);
}

void Scene::update()
{
  for (Model &model : models)
  {
    model.Draw(camera.getViewMatrix(), camera.getProjectionMatrix());
  }
}
