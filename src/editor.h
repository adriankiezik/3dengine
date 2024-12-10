#ifndef EDITOR_H
#define EDITOR_H

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "window.h"
#include <imfilebrowser.h>
#include "scene.h"
#include "script_system.h"

class Editor
{
public:
  explicit Editor(Window &window, Scene &scene, ScriptSystem &scriptSystem);
  ~Editor();

  void update();

private:
  Window &window;
  Scene &scene;
  ScriptSystem &scriptSystem;

  ImGui::FileBrowser modelDialog;
  ImGui::FileBrowser diffuseDialog;

  std::string modelPath;
  std::string diffusePath;

  // Renderer inside window
  GLuint fbo, texture, rbo;

  bool initialized = false;
  bool wireframe = false;
  bool vsync = true;
  int selectedScriptIndex;

  bool init();
  bool initImGui();
  void shutdownImGui();

  void renderMainMenu();
  void renderWireframeToggle();
  void renderVsyncToggle();

  void toggleWireframeMode();
};

#endif
