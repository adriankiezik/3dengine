#ifndef PROJECT_SELECTION_H
#define PROJECT_SELECTION_H

#include <imgui.h>
#include "imfilebrowser.h"

class ProjectSelection
{
public:
  ProjectSelection();
  void render();
  bool isProjectSelected() const { return projectSelected; }

private:
  bool projectSelected;
  ImGui::FileBrowser fileDialog;
};

#endif
