#include "hierarchy.h"
#include <imgui.h>
#include "imfilebrowser.h"

HierarchyWindow::HierarchyWindow(Scene &scene) : scene(scene), fileDialog(ImGuiFileBrowserFlags_EnterNewFilename)
{
  fileDialog.SetTitle("Select Script File");
  fileDialog.SetTypeFilters({".lua"});
}

void HierarchyWindow::render(bool &showHierarchy)
{
  if (!showHierarchy)
  {
    selectedObjectIndex = std::nullopt;
    showProperties = false;
    showScriptFileDialog = false;
    return;
  }

  ImGui::Begin("Hierarchy", &showHierarchy);
  std::vector<Object> &objects = scene.getObjects();
  renderHierarchyList(objects);
  ImGui::End();

  if (!showHierarchy)
  {
    selectedObjectIndex = std::nullopt;
    showProperties = false;
    showScriptFileDialog = false;
    return;
  }

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

  fileDialog.Display();
  if (fileDialog.HasSelected() && scriptTargetObjectIndex)
  {
    Object &object = scene.getObjects()[*scriptTargetObjectIndex];
    object.addScript(fileDialog.GetSelected().string());
    fileDialog.ClearSelected();
    scriptTargetObjectIndex = std::nullopt;
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

    // Handle right-click context menu
    if (ImGui::BeginPopupContextItem())
    {
      if (ImGui::MenuItem("Add Script"))
      {
        scriptTargetObjectIndex = i;
        fileDialog.Open();
      }
      if (ImGui::MenuItem("Delete"))
      {
        objects.erase(objects.begin() + i);
        if (selectedObjectIndex && *selectedObjectIndex == i)
        {
          selectedObjectIndex = std::nullopt;
          showProperties = false;
        }
        ImGui::EndPopup();
        break;
      }
      if (ImGui::MenuItem("Duplicate"))
      {
        Object newObject = object;
        newObject.setName(object.getName() + "_copy");
        objects.push_back(newObject);
      }
      ImGui::EndPopup();
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
  if (ImGui::CollapsingHeader("Transform"))
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

  // Scripts section
  if (ImGui::CollapsingHeader("Scripts"))
  {
    const auto &scripts = object.getScripts();
    if (scripts.empty())
    {
      ImGui::TextDisabled("No scripts attached");
    }
    else
    {
      for (size_t i = 0; i < scripts.size(); i++)
      {
        const auto &script = scripts[i];
        std::string label = script.name + "##script" + std::to_string(i);

        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
        ImGui::TreeNodeEx(label.c_str(), flags);

        if (ImGui::BeginPopupContextItem())
        {
          if (ImGui::MenuItem("Remove"))
          {
            object.removeScript(script.name);
          }
          ImGui::EndPopup();
        }
      }
    }
  }
}