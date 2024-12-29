#include "hierarchy.h"
#include <imgui.h>

HierarchyWindow::HierarchyWindow(Scene &scene) : scene(scene) {}

void HierarchyWindow::render(bool &showHierarchy)
{
  if (!showHierarchy)
    return;

  ImGui::Begin("Hierarchy", &showHierarchy);

  std::vector<Object> &objects = scene.getObjects();
  for (size_t i = 0; i < objects.size(); i++)
  {
    Object &object = objects[i];
    std::string label = object.getName() + "##" + std::to_string(i);

    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
    if (selectedObjectIndex && *selectedObjectIndex == i)
    {
      flags |= ImGuiTreeNodeFlags_Selected;
    }

    ImGui::TreeNodeEx(label.c_str(), flags);
    if (ImGui::IsItemClicked())
    {
      selectedObjectIndex = i;
      showProperties = true;
    }
  }

  ImGui::End();

  if (showProperties && selectedObjectIndex)
  {
    renderObjectProperties();
  }
}

void HierarchyWindow::renderObjectProperties()
{
  if (!selectedObjectIndex)
    return;

  std::vector<Object> &objects = scene.getObjects();
  if (*selectedObjectIndex >= objects.size())
    return;

  Object &object = objects[*selectedObjectIndex];

  ImGui::Begin("Object Properties", &showProperties);

  // Name
  static char nameBuffer[256];
  strncpy(nameBuffer, object.getName().c_str(), sizeof(nameBuffer) - 1);
  if (ImGui::InputText("Name", nameBuffer, sizeof(nameBuffer)))
  {
    object.setName(nameBuffer);
  }

  // Position
  glm::vec3 position = object.getPosition();
  if (ImGui::DragFloat3("Position", &position[0], 0.1f))
  {
    object.setPosition(position);
  }

  // Rotation
  glm::vec3 rotation = object.getRotation();
  if (ImGui::DragFloat3("Rotation", &rotation[0], 1.0f))
  {
    object.setRotation(rotation);
  }

  // Scale
  glm::vec3 scale = object.getScale();
  if (ImGui::DragFloat3("Scale", &scale[0], 0.1f))
  {
    object.setScale(scale);
  }

  ImGui::End();
}