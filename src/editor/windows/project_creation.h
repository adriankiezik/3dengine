#pragma once

#include <string>
#include "imgui.h"
#include "imgui_internal.h"
#include "imfilebrowser.h"

class ProjectSelection;

class ProjectCreationWindow
{
public:
  ProjectCreationWindow(ProjectSelection &projectSelection);
  void render();
  void show() { isVisible = true; }
  void hide() { isVisible = false; }
  bool isShown() const { return isVisible; }

private:
  ProjectSelection &projectSelection;
  std::string projectName;
  std::string projectDescription;
  std::string projectPath;
  ImGui::FileBrowser fileBrowser;
  bool isVisible;
  bool showFileBrowser;
  void createProject();
  void openFileBrowser();
};
