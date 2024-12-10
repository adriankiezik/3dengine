#include "editor.h"
#include <iostream>
#include <sstream>
#include "model.h"

Editor::Editor(Window &window, Scene &scene) : window(window), scene(scene)
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

  renderMainMenu();
  renderFPSMenu();

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Editor::renderMainMenu()
{
  ImGui::SetNextWindowPos(ImVec2(25, 25), ImGuiCond_Once);
  ImGui::SetNextWindowCollapsed(true, ImGuiCond_Once);
  ImGui::Begin("3dengine");

  if (ImGui::CollapsingHeader("Add model"))
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

void Editor::renderFPSMenu()
{
  ImGui::SetNextWindowPos(ImVec2(window.getWidth() - 150, 25), ImGuiCond_Once);
  ImGui::SetNextWindowCollapsed(false, ImGuiCond_Once);
  ImGui::SetNextWindowSize(ImVec2(125, 50));

  ImGui::Begin("FPS Counter");

  int currentFps = window.getFramesPerSecond();
  std::ostringstream oss;
  oss << currentFps;
  std::string fpsText = "FPS: " + oss.str();
  ImGui::Text("%s", fpsText.c_str());

  ImGui::End();
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
