#include "scene.h"
#include <iostream>

Scene::Scene(Window &window, Camera &camera) : window(window), camera(camera) {}

void Scene::addModel(const Model &model)
{
  models.push_back(model);
}

void Scene::draw()
{
  float aspectRatio = window.getWidth() / window.getHeight();

  for (Model &model : models)
  {
    model.Draw(camera.getViewMatrix(), camera.getProjectionMatrix(aspectRatio));
  }
}
