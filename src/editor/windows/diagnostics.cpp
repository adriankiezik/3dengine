#include "diagnostics.h"
#include <imgui.h>

DiagnosticsWindow::DiagnosticsWindow() {}

void DiagnosticsWindow::render(bool &showDiagnostics)
{
  if (!showDiagnostics)
    return;

  ImGui::Begin("Diagnostics", &showDiagnostics);

  ImGui::End();
}