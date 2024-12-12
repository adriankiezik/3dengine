#ifndef MENU_BAR_H
#define MENU_BAR_H

#include <imgui.h>
#include <string>
#include "../window.h"

class MenuBar
{
public:
  MenuBar(Window &window);
  void render();
  void openBrowser(const char *url);

  bool showConsole;
  bool showHierarchy;
  bool showScripts;
  bool showDiagnostics;

private:
  Window &window;
};

#endif
