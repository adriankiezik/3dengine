#include "scene.h"
#include <iostream>

void Scene::addModel(const Model &model)
{
  models.push_back(model);
}

void Scene::draw(Shader &shader)
{
  for (Model &model : models)
  {
    model.Draw(shader);
  }
}
