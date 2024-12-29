#pragma once

#include "script_viewer.h"
#include "../../scene.h"
#include <vector>
#include <string>

class ScriptsWindow
{
public:
  explicit ScriptsWindow(Scene& scene);
  void render(bool& showScripts);

private:
  Scene& scene;
  ScriptViewerWindow scriptViewer;
  bool showViewer = false;
};
