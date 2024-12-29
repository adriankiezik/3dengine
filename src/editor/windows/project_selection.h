#ifndef PROJECT_SELECTION_H
#define PROJECT_SELECTION_H

#include <imgui.h>
#include "imfilebrowser.h"
#include <string>
#include <map>
#include "../../window.h"

class ProjectCreationWindow;

class ProjectSelection
{
public:
  ProjectSelection(ProjectCreationWindow &projectCreationWindow, Window &window);
  void render();
  bool isProjectSelected() const { return projectSelected; }
  void setProjectSelected(bool selected);
  void setProjectName(const std::string &name);
  void setProjectDescription(const std::string &description);

private:
  Window &window;
  bool projectSelected;

  std::string projectName;
  std::string projectDescription;

  ImGui::FileBrowser fileDialog;

  ProjectCreationWindow &projectCreationWindow;
};

#endif
