#ifndef EDITOR_H
#define EDITOR_H

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "window.h"

class Editor
{
public:
  explicit Editor(Window &window);
  ~Editor();

  bool init();
  void update();

private:
  Window &window;

  bool initialized = false;
  bool wireframe = false;
  bool vsync = true;

  bool initImGui();
  void shutdownImGui();

  void renderMainMenu();
  void renderFPSMenu();
  void renderWireframeToggle();
  void renderVsyncToggle();

  void toggleWireframeMode();
};

#endif
