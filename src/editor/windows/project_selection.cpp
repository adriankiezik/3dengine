#include "project_selection.h"

ProjectSelection::ProjectSelection() : projectSelected(false) {}

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
            // TODO: Implement project opening
            projectSelected = true;
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}
