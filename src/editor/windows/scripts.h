#pragma once

#include "script_viewer.h"
#include "../../scene.h"
#include <vector>
#include <string>

class ScriptsWindow
{
public:
  ScriptsWindow(Scene& scene);
  void init();
  void render(bool& showScripts);

private:
  Scene& scene;
  ScriptViewerWindow scriptViewer;
  bool showViewer = false;
};
