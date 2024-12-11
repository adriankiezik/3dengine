#ifndef SCENE_H
#define SCENE_H

#include "model.h"
#include <vector>
#include "camera.h"
#include "window.h"
#include "framebuffer.h"
#include "grid.h"

class Scene
{
public:
  Scene(Camera &camera, Framebuffer &framebuffer);
  void addModel(const Model &model);
  void update();

private:
  Camera &camera;
  Framebuffer &framebuffer;
  std::vector<Model> models;

  Grid grid;
};

#endif
