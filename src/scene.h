#ifndef SCENE_H
#define SCENE_H

#include "object.h"
#include <vector>
#include "camera.h"
#include "window.h"
#include "framebuffer.h"
#include "grid.h"
#include <memory>
#include <nlohmann/json.hpp>

class Scene
{
public:
  Scene(Camera &camera, Framebuffer &framebuffer);
  void addObject(const Object &object);
  void update();
  const std::vector<Object> &getObjects() const { return objects; }
  std::vector<Object> &getObjects() { return objects; }
  void removeObject(size_t index);
  void clear();

  // Serialization methods
  nlohmann::json toJson() const;
  void fromJson(const nlohmann::json &json);

private:
  Camera &camera;
  Framebuffer &framebuffer;
  std::vector<Object> objects;
  Grid grid;
};

#endif
