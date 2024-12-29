#ifndef PROJECT_SELECTION_H
#define PROJECT_SELECTION_H

#include <imgui.h>
#include "imfilebrowser.h"
#include <string>
#include "../../window.h"

class ProjectCreationWindow;

class ProjectSelection
{
public:
  ProjectSelection(ProjectCreationWindow &projectCreationWindow, Window &window);
  void render();
  bool isProjectSelected() const { return projectSelected; }
  void setProjectSelected(bool selected) { projectSelected = selected; }
  void setProjectName(const std::string &name) { projectName = name; }
  void setProjectDescription(const std::string &description) { projectDescription = description; }
  void setProjectPath(const std::string &path) { projectPath = path; }

private:
  Window &window;
  bool projectSelected;

  std::string projectName;
  std::string projectDescription;
  std::string projectPath;

  ImGui::FileBrowser fileDialog;

  ProjectCreationWindow &projectCreationWindow;
};

#endif
