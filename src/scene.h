#ifndef SCENE_H
#define SCENE_H

#include "model.h"
#include <vector>

class Scene
{
public:
  void addModel(const Model &model);
  void draw(Shader &shader);

private:
  std::vector<Model> models;
};

#endif
