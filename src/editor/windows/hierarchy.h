#ifndef HIERARCHY_WINDOW_H
#define HIERARCHY_WINDOW_H

#include "../../scene.h"
#include <optional>

class HierarchyWindow
{
public:
  HierarchyWindow(Scene &scene);
  void render(bool &showHierarchy);

private:
  void renderObjectProperties();
  
  Scene &scene;
  std::optional<size_t> selectedObjectIndex;
  bool showProperties = false;
};

#endif
