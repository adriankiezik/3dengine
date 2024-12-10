#ifndef SCENE_H
#define SCENE_H

#include "model.h"
#include <vector>
#include "camera.h"
#include "window.h"

class Scene
{
public:
  Scene(Window &window, Camera &camera);
  void addModel(const Model &model);
  void update();

private:
  Window &window;
  Camera &camera;
  std::vector<Model> models;
};

#endif
