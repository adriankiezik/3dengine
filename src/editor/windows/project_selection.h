#ifndef PROJECT_SELECTION_H
#define PROJECT_SELECTION_H

#include <imgui.h>

class ProjectSelection
{
public:
    ProjectSelection();
    void render();
    bool isProjectSelected() const { return projectSelected; }

private:
    bool projectSelected;
};

#endif
