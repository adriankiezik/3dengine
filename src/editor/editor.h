#ifndef EDITOR_H
#define EDITOR_H

#include "menu_bar.h"
#include "windows/viewport.h"
#include "windows/console.h"
#include "windows/hierarchy.h"
#include "windows/diagnostics.h"
#include "windows/scripts.h"
#include "../window.h"
#include "../scene.h"
#include "../script_system.h"
#include "../console_stream_buffer.h"

class Editor
{
public:
  explicit Editor(Window &window, Scene &scene, Camera &camera,
                  ScriptSystem &scriptSystem, Framebuffer &framebuffer,
                  ConsoleStreamBuffer &console);
  ~Editor();

  void update();

private:
  Window &window;
  Scene &scene;
  Camera &camera;
  ScriptSystem &scriptSystem;
  Framebuffer &framebuffer;
  ConsoleStreamBuffer &console;

  bool initialized = false;

  MenuBar menuBar;
  ViewportWindow viewportWindow;
  ConsoleWindow consoleWindow;
  HierarchyWindow hierarchyWindow;
  ScriptsWindow scriptsWindow;
  DiagnosticsWindow diagnosticsWindow;

  bool init();
  void shutdown();
  void startFrame();
  void endFrame();
};

#endif
