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
#include <functional>
#include <string>
#include <vector>

struct ImVec3
{
  float x, y, z;
  ImVec3(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f)
  {
    x = _x;
    y = _y;
    z = _z;
  }
};

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
  Framebuffer &framebuffer;

  ImVec2 renderer_size;
  bool initialized = false;

  ImGui::FileBrowser modelDialog;
  ImGui::FileBrowser diffuseDialog;
  std::string modelPath;
  std::string diffusePath;

  bool wireframe = false;
  bool vsync = true;
  int selectedScriptIndex = -1;

  bool showConsole;
  bool showScripts;
  bool showHierarchy;
  bool showDiagnostics;

  bool init();
  bool initImGui();
  void shutdownImGui();

  void startFrame();
  void endFrame();

  void renderEditorUI();

  void renderViewport();
  void renderMainMenu();
  void renderMenuBar();

  void renderConsole();
  void renderScripts();
  void renderHierarchy();
  void renderDiagnostics();

  void renderObjectsMenu();
  void renderScriptsMenu();
  void renderDiagnosticsMenu();
  void renderSettingsMenu();

  void adjustViewportSize();
  void renderFileSelector(const char *label, std::string &path, ImGui::FileBrowser &dialog, const std::vector<std::string> &filters);
  void renderCenteredButton(const char *label, std::function<void()> onClick);
  void handleDialogSelections();
  void addModelToScene();

  void renderWireframeToggle();
  void toggleWireframeMode();
  void renderVsyncToggle();

  void openBrowser(const char *url);

  void setupImGuiStyle();
  void styleImGuiComponents(
      ImVec3 color_for_text, ImVec3 color_for_head,
      ImVec3 color_for_area, ImVec3 color_for_body, ImVec3 color_for_pops);
};

#endif
