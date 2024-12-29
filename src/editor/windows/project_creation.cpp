#include "project_creation.h"
#include <imgui.h>
#include <fstream>
#include <filesystem>
#include <nlohmann/json.hpp>
#include "imfilebrowser.h"
#include "project_selection.h"

ProjectCreationWindow::ProjectCreationWindow(ProjectSelection &projectSelection)
    : projectSelection(projectSelection),
      projectName(""),
      projectDescription(""),
      projectPath(std::filesystem::current_path().string()),
      isVisible(false),
      showFileBrowser(false),
      fileBrowser(ImGuiFileBrowserFlags_SelectDirectory | ImGuiFileBrowserFlags_CreateNewDir)
{
  fileBrowser.SetTitle("Choose Project Location");
  fileBrowser.SetTypeFilters({});
}

void ProjectCreationWindow::render()
{
  if (!isVisible)
    return;

  ImGui::SetNextWindowSize(ImVec2(500, 370));
  ImGui::Begin("Create New Project", &isVisible, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

  // Center window on screen
  ImGui::SetWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f - ImGui::GetWindowWidth() * 0.5f,
                             ImGui::GetIO().DisplaySize.y * 0.5f - ImGui::GetWindowHeight() * 0.5f));

  ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8, 6));
  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 10));

  // Title
  ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
  ImGui::TextColored(ImVec4(0.28f, 0.56f, 1.0f, 1.0f), "Project Details");
  ImGui::PopFont();
  ImGui::Spacing();
  ImGui::Separator();
  ImGui::Spacing();

  // Project Name
  char nameBuffer[256] = "";
  strncpy(nameBuffer, projectName.c_str(), sizeof(nameBuffer) - 1);
  ImGui::TextUnformatted("Name");
  if (ImGui::InputTextWithHint("##name", "Enter project name", nameBuffer, sizeof(nameBuffer)))
  {
    projectName = nameBuffer;
  }

  ImGui::Spacing();

  // Project Description
  ImGui::TextUnformatted("Description");
  char descBuffer[1024] = "";
  strncpy(descBuffer, projectDescription.c_str(), sizeof(descBuffer) - 1);
  if (ImGui::InputTextMultiline("##desc", descBuffer, sizeof(descBuffer), ImVec2(-1, 60)))
  {
    projectDescription = descBuffer;
  }

  ImGui::Spacing();

  // Project Path
  ImGui::TextUnformatted("Location");
  char pathBuffer[512] = "";
  strncpy(pathBuffer, projectPath.c_str(), sizeof(pathBuffer) - 1);

  ImGui::PushItemWidth(ImGui::GetWindowWidth() - 120);
  if (ImGui::InputTextWithHint("##path", "Project path", pathBuffer, sizeof(pathBuffer)))
  {
    projectPath = pathBuffer;
  }
  ImGui::PopItemWidth();

  ImGui::SameLine();
  ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.28f, 0.56f, 1.0f, 0.6f));
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.28f, 0.56f, 1.0f, 1.0f));
  if (ImGui::Button("Browse..."))
  {
    openFileBrowser();
  }
  ImGui::PopStyleColor(2);

  ImGui::Spacing();
  ImGui::Spacing();

  // Create Button
  ImGui::SetCursorPosX((ImGui::GetWindowWidth() - 120) * 0.5f);
  ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.28f, 0.56f, 1.0f, 0.6f));
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.28f, 0.56f, 1.0f, 1.0f));
  if (ImGui::Button("Create Project", ImVec2(120, 30)))
  {
    createProject();
    hide();
  }
  ImGui::PopStyleColor(2);

  ImGui::PopStyleVar(2);

  if (showFileBrowser)
  {
    fileBrowser.Display();

    if (fileBrowser.HasSelected())
    {
      projectPath = fileBrowser.GetSelected().string();
      showFileBrowser = false;
      fileBrowser.ClearSelected();
    }

    if (!fileBrowser.IsOpened())
    {
      showFileBrowser = false;
    }
  }

  ImGui::End();
}

void ProjectCreationWindow::createProject()
{
  if (projectName.empty() || projectPath.empty())
    return;

  std::filesystem::path projectDir(projectPath);
  if (!std::filesystem::exists(projectDir))
  {
    std::filesystem::create_directories(projectDir);

    // Create project subdirectories
    auto assetsDir = projectDir / "assets";
    auto scenesDir = projectDir / "scenes";
    auto scriptsDir = projectDir / "scripts";

    std::filesystem::create_directories(assetsDir);
    std::filesystem::create_directories(scenesDir);
    std::filesystem::create_directories(scriptsDir);
  }

  std::string projectFile = (projectDir / (projectName + ".3dproj")).string();
  std::ofstream file(projectFile);
  if (file.is_open())
  {
    std::filesystem::path projectFilePath(projectFile);
    auto projectFileDir = projectFilePath.parent_path();

    nlohmann::ordered_json projectData = {
        {"ProjectName", projectName},
        {"Description", projectDescription},
    };

    file << projectData.dump(4);
    file.close();

    projectSelection.setProjectName(projectName);
    projectSelection.setProjectDescription(projectDescription);

    hide();
    projectSelection.setProjectSelected(true);
  }
}

void ProjectCreationWindow::openFileBrowser()
{
  showFileBrowser = true;
  fileBrowser.Open();
}
