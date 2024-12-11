#include "editor.h"
#include <iostream>
#include <sstream>
#include "model.h"
#include "script_system.h"
#include "camera.h"

Editor::Editor(Window &window, Scene &scene, Camera &camera, ScriptSystem &scriptSystem, Framebuffer &framebuffer)
    : window(window), scene(scene), camera(camera), scriptSystem(scriptSystem), framebuffer(framebuffer), selectedScriptIndex(-1)
{
  init();
}

Editor::~Editor()
{
  if (initialized)
  {
    shutdownImGui();
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
  initialized = false;
}

void Editor::update()
{
  startFrame();
  renderEditorUI();
  endFrame();
}

void Editor::startFrame()
{
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void Editor::endFrame()
{
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Editor::renderEditorUI()
{
  renderViewport();
  renderMenuBar();
  // renderMainMenu();
}

void Editor::renderMenuBar()
{
  if (ImGui::BeginMainMenuBar())
  {
    if (ImGui::BeginMenu("File"))
    {
      if (ImGui::MenuItem("New"))
      {
      }
      if (ImGui::MenuItem("Save"))
      {
      }
      if (ImGui::MenuItem("Settings"))
      {
      }
      if (ImGui::MenuItem("Exit"))
      {
      }
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Edit"))
    {
      if (ImGui::MenuItem("Add Script"))
      {
      }
      if (ImGui::MenuItem("Add Object"))
      {
      }
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Windows"))
    {
      if (ImGui::MenuItem("Console"))
      {
      }
      if (ImGui::MenuItem("Hierarchy"))
      {
      }
      if (ImGui::MenuItem("Diagnostics"))
      {
      }
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Help"))
    {
      if (ImGui::MenuItem("Documentation"))
      {
      }
      if (ImGui::MenuItem("Github"))
      {
      }
      ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
  }
}

void Editor::renderViewport()
{
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
  ImGui::Begin("Renderer");

  adjustViewportSize();

  ImVec2 pos = ImGui::GetCursorScreenPos();
  ImGui::GetWindowDrawList()->AddImage(
      (ImTextureID)(uintptr_t)framebuffer.getTextureId(),
      ImVec2(pos.x, pos.y),
      ImVec2(pos.x + renderer_size.x, pos.y + renderer_size.y),
      ImVec2(0, 1),
      ImVec2(1, 0));

  ImGui::End();
  ImGui::PopStyleVar();
}

void Editor::adjustViewportSize()
{
  const float window_width = ImGui::GetContentRegionAvail().x;
  const float window_height = ImGui::GetContentRegionAvail().y;

  if (window_width != renderer_size.x || window_height != renderer_size.y)
  {
    renderer_size = ImVec2(window_width, window_height);
    window.getFramebuffer().rescale(window_width, window_height);
    camera.setAspectRatio(static_cast<float>(window_width / window_height));
  }

  glViewport(0, 0, window_width, window_height);
}

void Editor::renderMainMenu()
{
  ImGui::SetNextWindowPos(ImVec2(25, 25), ImGuiCond_Once);
  ImGui::SetNextWindowCollapsed(true, ImGuiCond_Once);
  ImGui::Begin("3dengine");

  renderObjectsMenu();
  renderScriptsMenu();
  renderDiagnosticsMenu();
  renderSettingsMenu();

  ImGui::End();

  handleDialogSelections();
}

void Editor::renderObjectsMenu()
{
  if (!ImGui::CollapsingHeader("Objects"))
    return;

  renderFileSelector("Model", modelPath, modelDialog, {".obj", ".fbx"});
  renderFileSelector("Diffuse texture", diffusePath, diffuseDialog, {".png", ".jpg"});

  ImGui::Spacing();
  renderCenteredButton("Add to scene", [&]()
                       { addModelToScene(); });
}

void Editor::renderFileSelector(const char *label, std::string &path, ImGui::FileBrowser &dialog, const std::vector<std::string> &filters)
{
  ImGui::Text("%s:", label);
  ImGui::SameLine();
  ImGui::PushID(label);

  const char *labelText = path.empty() ? "None" : path.c_str();
  if (ImGui::Button(labelText))
  {
    dialog.SetTitle(std::string("Select ") + label);
    dialog.SetTypeFilters(filters);
    dialog.Open();
  }

  ImGui::PopID();
}

void Editor::renderCenteredButton(const char *label, std::function<void()> onClick)
{
  float window_width = ImGui::GetWindowWidth();
  float button_width = 100.0f;
  float x_center = (window_width - button_width) * 0.5f;

  ImGui::SetCursorPosX(x_center);
  if (ImGui::Button(label))
  {
    onClick();
  }
}

void Editor::addModelToScene()
{
  std::vector<std::pair<std::string, std::string>> texturePaths = {{"texture_diffuse", diffusePath}};
  Model newModel(modelPath, texturePaths, "../shaders/vertex_shader.glsl", "../shaders/fragment_shader.glsl");
  scene.addModel(newModel);

  modelPath.clear();
  diffusePath.clear();
}

void Editor::renderScriptsMenu()
{
  if (!ImGui::CollapsingHeader("Scripts"))
    return;

  auto scripts = scriptSystem.getScripts();

  ImGui::PushID("ScriptsList");
  if (!scripts.empty() && ImGui::BeginListBox("", ImVec2(ImGui::GetWindowWidth(), ImGui::GetTextLineHeightWithSpacing() * scripts.size())))
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
}

void Editor::renderDiagnosticsMenu()
{
  if (!ImGui::CollapsingHeader("Diagnostics"))
    return;

  int currentFps = window.getFramesPerSecond();
  ImGui::Text("FPS: %d", currentFps);
}

void Editor::renderSettingsMenu()
{
  if (!ImGui::CollapsingHeader("Settings"))
    return;

  renderWireframeToggle();
  renderVsyncToggle();
}

void Editor::handleDialogSelections()
{
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
  ImGui::Checkbox("Wireframe", &wireframe);
  toggleWireframeMode();
}

void Editor::toggleWireframeMode()
{
  glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
}

void Editor::renderVsyncToggle()
{
  ImGui::Checkbox("Vertical Sync", &vsync);
  glfwSwapInterval(vsync ? 1 : 0);
}