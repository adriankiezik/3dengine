#ifndef EDITOR_H
#define EDITOR_H

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "window.h"
#include <imfilebrowser.h>
#include "scene.h"

class Editor
{
public:
  explicit Editor(Window &window, Scene &scene);
  ~Editor();

  bool init();
  void update();

private:
  Window &window;
  Scene &scene;

  ImGui::FileBrowser modelDialog;
  ImGui::FileBrowser diffuseDialog;

  std::string modelPath;
  std::string diffusePath;

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
