#include "scripts.h"
#include <imgui.h>
#include "../../script.h"

ScriptsWindow::ScriptsWindow(Scene& scene) : scene(scene) {}

void ScriptsWindow::render(bool& showScripts)
{
  if (!showScripts)
    return;

  ImGui::Begin("Scripts", &showScripts);

  // Get all objects with scripts
  auto& objects = scene.getObjects();
  for (size_t i = 0; i < objects.size(); i++) {
    auto& object = objects[i];
    const auto& scripts = object.getScripts();
    if (!scripts.empty()) {
      if (ImGui::TreeNode(object.getName().c_str())) {
        for (const auto& script : scripts) {
          if (ImGui::Selectable(script.name.c_str())) {
            scriptViewer.setScript(script.path, script.content);
            showViewer = true;
          }
        }
        ImGui::TreePop();
      }
    }
  }

  ImGui::End();

  // Render script viewer if active
  scriptViewer.render(showViewer);
}