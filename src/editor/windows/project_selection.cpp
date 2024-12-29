#include "project_selection.h"
#include "imfilebrowser.h"

ProjectSelection::ProjectSelection() : fileDialog(0), projectSelected(false)
{
  fileDialog.SetTitle("Select Project File");
  fileDialog.SetTypeFilters({".3dproj"});
}

void ProjectSelection::render()
{
  ImGui::OpenPopup("Project Selection");

  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));

  if (ImGui::BeginPopupModal("Project Selection", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
  {
    ImGui::Text("Welcome to the 3D Engine!");
    ImGui::Separator();

    if (ImGui::Button("Create New Project", ImVec2(200, 0)))
    {
      // TODO: Implement new project creation
      projectSelected = true;
      ImGui::CloseCurrentPopup();
    }

    ImGui::Spacing();

    if (ImGui::Button("Open Existing Project", ImVec2(200, 0)))
    {
      fileDialog.Open();
    }
  }

  fileDialog.Display();

  if (fileDialog.HasSelected())
  {
    std::filesystem::path selectedPath = fileDialog.GetSelected();
    // TODO: Load the project from the selected path
    fileDialog.ClearSelected();
    projectSelected = true;
  }

  ImGui::EndPopup();
}
