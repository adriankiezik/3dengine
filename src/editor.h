#ifndef EDITOR_H
#define EDITOR_H

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "window.h"
#include <imfilebrowser.h>
#include "scene.h"
#include "script_system.h"
#include "framebuffer.h"
#include "camera.h"

class Editor
{
public:
  explicit Editor(Window &window, Scene &scene, Camera &camera, ScriptSystem &scriptSystem, Framebuffer &framebuffer);
  ~Editor();

  void update();

private:
  Window &window;
  Scene &scene;
  Camera &camera;
  ScriptSystem &scriptSystem;

  GLuint fbo, texture, rbo;
  Framebuffer &framebuffer;
  ImVec2 renderer_size;

  ImGui::FileBrowser modelDialog;
  ImGui::FileBrowser diffuseDialog;

  std::string modelPath;
  std::string diffusePath;

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
