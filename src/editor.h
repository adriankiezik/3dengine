#ifndef EDITOR_H
#define EDITOR_H

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "window.h"

class Editor {
public:
  Editor(Window &window);
  ~Editor();

  bool initialize();
  void shutdown();
  void update();

private:
  Window &m_window;

  bool m_initialized = false;
  bool m_wireframe = false;
  bool m_vsync = true;

  void renderMainMenu();
  void renderFPSMenu();
  void renderVsyncToggle();
  void renderWireframeToggle();
};

#endif
