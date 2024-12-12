#include "hierarchy.h"
#include <imgui.h>

HierarchyWindow::HierarchyWindow() {}

void HierarchyWindow::render(bool &showHierarchy)
{
  if (!showHierarchy)
    return;

  ImGui::Begin("Hierarchy", &showHierarchy);

  ImGui::End();
}