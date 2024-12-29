#include "hierarchy.h"
#include <imgui.h>

HierarchyWindow::HierarchyWindow(Scene &scene) : scene(scene) {}

void HierarchyWindow::render(bool &showHierarchy)
{
  if (!showHierarchy)
  {
    selectedObjectIndex = std::nullopt;
    showProperties = false;
    return;
  }

  // Render hierarchy window
  ImGui::Begin("Hierarchy", &showHierarchy);
  std::vector<Object> &objects = scene.getObjects();
  renderHierarchyList(objects);
  ImGui::End();

  // Handle window closing
  if (!showHierarchy)
  {
    selectedObjectIndex = std::nullopt;
    showProperties = false;
    return;
  }

  // Render properties window if object is selected
  if (showProperties && selectedObjectIndex && *selectedObjectIndex < objects.size())
  {
    Object &object = objects[*selectedObjectIndex];
    ImGui::Begin("Object Properties", &showProperties);
    renderObjectProperties(object);
    ImGui::End();

    if (!showProperties)
    {
      selectedObjectIndex = std::nullopt;
    }
  }
}

void HierarchyWindow::renderHierarchyList(std::vector<Object> &objects)
{
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
}

void HierarchyWindow::renderObjectProperties(Object &object)
{
  static char nameBuffer[256];
  strncpy(nameBuffer, object.getName().c_str(), sizeof(nameBuffer) - 1);
  if (ImGui::InputText("Name", nameBuffer, sizeof(nameBuffer)))
  {
    object.setName(nameBuffer);
  }

  renderTransformProperties(object);
}

void HierarchyWindow::renderTransformProperties(Object &object)
{
  glm::vec3 position = object.getPosition();
  if (ImGui::DragFloat3("Position", &position[0], 0.1f))
  {
    object.setPosition(position);
  }

  glm::vec3 rotation = object.getRotation();
  if (ImGui::DragFloat3("Rotation", &rotation[0], 1.0f))
  {
    object.setRotation(rotation);
  }

  glm::vec3 scale = object.getScale();
  if (ImGui::DragFloat3("Scale", &scale[0], 0.1f))
  {
    object.setScale(scale);
  }
}