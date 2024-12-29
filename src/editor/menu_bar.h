#ifndef MENU_BAR_H
#define MENU_BAR_H

#include <imgui.h>
#include <string>
#include "../window.h"
#include "../scene.h"
#include "../camera.h"

class MenuBar
{
public:
  MenuBar(Window &window, Scene &scene, Camera &camera);
  void render();
  void openBrowser(const char *url);
  void setCurrentProjectPath(const std::string &path) { currentProjectPath = path; }

  bool showConsole;
  bool showHierarchy;
  bool showScripts;
  bool showDiagnostics;

private:
  void saveScene();

  Window &window;
  Scene &scene;
  Camera &camera;
  std::string currentProjectPath;
};

#endif
