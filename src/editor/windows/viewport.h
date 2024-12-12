#ifndef VIEWPORT_WINDOW_H
#define VIEWPORT_WINDOW_H

#include <imgui.h>
#include "../../framebuffer.h"
#include "../../camera.h"

class ViewportWindow
{
public:
  explicit ViewportWindow(Framebuffer &framebuffer, Camera &camera);
  void render();
  void adjustViewportSize();

private:
  Framebuffer &framebuffer;
  Camera &camera;
  ImVec2 renderer_size;
};

#endif
