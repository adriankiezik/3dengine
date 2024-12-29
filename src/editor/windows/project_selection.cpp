#include "project_selection.h"
#include "project_creation.h"
#include "imfilebrowser.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <filesystem>

ProjectSelection::ProjectSelection(ProjectCreationWindow &projectCreationWindow, Window &window) : projectCreationWindow(projectCreationWindow), fileDialog(0), projectSelected(false), window(window)
{
  fileDialog.SetTitle("Select Project File");
  fileDialog.SetTypeFilters({".3dproj"});

  const char *homeDir = std::getenv("HOME");
  if (homeDir != nullptr)
  {
    std::filesystem::path documentsPath = std::filesystem::path(homeDir) / "Documents";
    if (std::filesystem::exists(documentsPath))
    {
      fileDialog.SetPwd(documentsPath);
    }
  }
}

void ProjectSelection::render()
{
  if (projectCreationWindow.isShown())
    return;

  ImGui::Begin("Project Selection", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

  ImGui::SetWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f - ImGui::GetWindowWidth() * 0.5f,
                             ImGui::GetIO().DisplaySize.y * 0.5f - ImGui::GetWindowHeight() * 0.5f));

  ImGui::Text("Welcome to the 3dengine!");
  ImGui::Separator();

  if (ImGui::Button("Create New Project", ImVec2(200, 0)))
  {
    projectCreationWindow.show();
  }

  if (ImGui::Button("Open Existing Project", ImVec2(200, 0)))
  {
    fileDialog.Open();
  }

  fileDialog.Display();

  if (fileDialog.HasSelected())
  {
    std::filesystem::path selectedPath = fileDialog.GetSelected();

    // Load and parse the project file
    std::ifstream file(selectedPath);
    if (file.is_open())
    {
      try
      {
        nlohmann::json projectData = nlohmann::json::parse(file);

        setProjectName(projectData["ProjectName"]);
        setProjectDescription(projectData["Description"]);
        setProjectPath(projectData["Path"]);

        window.setTitle("3dengine - " + projectData["ProjectName"].get<std::string>());

        fileDialog.ClearSelected();
        projectSelected = true;
      }
      catch (const std::exception &e)
      {
        std::cerr << "Error loading project: " << e.what() << std::endl;
      }
      file.close();
    }
  }

  ImGui::End();
}
