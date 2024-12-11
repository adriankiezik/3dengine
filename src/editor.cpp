#include "editor.h"
#include <iostream>
#include <sstream>
#include "model.h"
#include "script_system.h"

Editor::Editor(Window &window, Scene &scene, ScriptSystem &scriptSystem, Framebuffer &framebuffer)
    : window(window), scene(scene), scriptSystem(scriptSystem), selectedScriptIndex(-1), framebuffer(framebuffer)
{
  init();
}

Editor::~Editor()
{
  if (initialized)
  {
    shutdownImGui();
    initialized = false;
  }
}

bool Editor::init()
{
  return initImGui();
}

bool Editor::initImGui()
{
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();

  ImGui::StyleColorsDark();

  ImGui_ImplGlfw_InitForOpenGL(window.getWindow(), true);
  ImGui_ImplOpenGL3_Init("#version 330");

  initialized = true;
  return true;
}

void Editor::shutdownImGui()
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void Editor::update()
{
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  ImGui::Begin("My Scene");

  // we access the ImGui window size
  const float window_width = ImGui::GetContentRegionAvail().x;
  const float window_height = ImGui::GetContentRegionAvail().y;

  // we rescale the framebuffer to the actual window size here and reset the glViewport
  framebuffer.rescale(window_width, window_height);
  glViewport(0, 0, window_width, window_height);

  // we get the screen position of the window
  ImVec2 pos = ImGui::GetCursorScreenPos();

  // and here we can add our created texture as image to ImGui
  // unfortunately we need to use the cast to void* or I didn't find another way tbh
  ImGui::GetWindowDrawList()->AddImage(
      (ImTextureID)(uintptr_t)framebuffer.getTextureId(), // Correct type cast
      ImVec2(pos.x, pos.y),
      ImVec2(pos.x + window_width, pos.y + window_height),
      ImVec2(0, 1),
      ImVec2(1, 0));

  ImGui::End();

  renderMainMenu();

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Editor::renderMainMenu()
{
  ImGui::SetNextWindowPos(ImVec2(25, 25), ImGuiCond_Once);
  ImGui::SetNextWindowCollapsed(true, ImGuiCond_Once);
  ImGui::Begin("3dengine");

  if (ImGui::CollapsingHeader("Objects"))
  {
    ImGui::Text("Model:");
    ImGui::SameLine();
    ImGui::PushID(1);
    const char *modelLabel = modelPath.empty() ? "None" : modelPath.c_str();
    if (ImGui::Button(modelLabel))
    {
      modelDialog.SetTitle("Select model (.obj / .fbx)");
      modelDialog.SetTypeFilters({".obj", ".fbx"});
      modelDialog.Open();
    }
    ImGui::PopID();

    ImGui::Text("Diffuse texture:");
    ImGui::SameLine();
    ImGui::PushID(2);
    const char *diffuseLabel = diffusePath.empty() ? "None" : diffusePath.c_str();
    if (ImGui::Button(diffuseLabel))
    {
      diffuseDialog.SetTitle("Select diffuse texture (.png / .jpg)");
      diffuseDialog.SetTypeFilters({".png", ".jpg"});
      diffuseDialog.Open();
    }
    ImGui::PopID();

    ImGui::Spacing();

    float window_width = ImGui::GetWindowWidth();
    float button_width = 100.0f;
    float x_center = (window_width - button_width) * 0.5f;

    ImGui::SetCursorPosX(x_center);

    if (ImGui::Button("Add to scene"))
    {
      std::vector<std::pair<std::string, std::string>> texturePaths = {
          {"texture_diffuse", diffusePath}};

      Model newModel(modelPath, texturePaths, "../shaders/vertex_shader.glsl", "../shaders/fragment_shader.glsl");
      scene.addModel(newModel);

      modelPath = "";
      diffusePath = "";
    }
  }

  if (ImGui::CollapsingHeader("Scripts"))
  {
    std::vector<Script> scripts = scriptSystem.getScripts();

    ImGui::PushID(3);

    if (scripts.size() != 0 && ImGui::BeginListBox("", ImVec2(ImGui::GetWindowWidth(), ImGui::GetTextLineHeightWithSpacing() * scripts.size())))
    {
      for (int i = 0; i < scripts.size(); i++)
      {
        if (ImGui::Selectable(scripts[i].name.c_str(), selectedScriptIndex == i))
        {
          selectedScriptIndex = i;
        }
      }
      ImGui::EndListBox();
    }

    ImGui::PopID();

    if (selectedScriptIndex >= 0 && selectedScriptIndex < scripts.size())
    {
    }
  }

  if (ImGui::CollapsingHeader("Diagnostics"))
  {
    int currentFps = window.getFramesPerSecond();
    std::ostringstream oss;
    oss << currentFps;
    std::string fpsText = "FPS: " + oss.str();
    ImGui::Text("%s", fpsText.c_str());
  }

  if (ImGui::CollapsingHeader("Settings"))
  {
    renderWireframeToggle();
    renderVsyncToggle();
  }

  ImGui::End();

  modelDialog.Display();
  diffuseDialog.Display();

  if (modelDialog.HasSelected())
  {
    modelPath = modelDialog.GetSelected().string();
    modelDialog.ClearSelected();
  }

  if (diffuseDialog.HasSelected())
  {
    diffusePath = diffuseDialog.GetSelected().string();
    diffuseDialog.ClearSelected();
  }
}

void Editor::renderWireframeToggle()
{
  toggleWireframeMode();
  ImGui::Checkbox("Wireframe", &wireframe);
}

void Editor::toggleWireframeMode()
{
  glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
}

void Editor::renderVsyncToggle()
{
  glfwSwapInterval(vsync ? 1 : 0);
  ImGui::Checkbox("Vertical Sync", &vsync);
}
