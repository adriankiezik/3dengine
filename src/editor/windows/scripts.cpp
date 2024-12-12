#include "scripts.h"
#include <imgui.h>

ScriptsWindow::ScriptsWindow() {}

void ScriptsWindow::render(bool &showScripts)
{
  if (!showScripts)
    return;

  ImGui::Begin("Scripts", &showScripts);

  ImGui::End();
}