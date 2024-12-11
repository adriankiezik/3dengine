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

  // TBD
  // setupImGuiStyle();

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
        showConsole = true;
      }
      if (ImGui::MenuItem("Hierarchy"))
      {
        showHierarchy = true;
      }
      if (ImGui::MenuItem("Scripts"))
      {
        showScripts = true;
      }
      if (ImGui::MenuItem("Diagnostics"))
      {
        showDiagnostics = true;
      }
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Help"))
    {
      if (ImGui::MenuItem("Documentation"))
      {
        // Add link when docs are ready
      }
      if (ImGui::MenuItem("Github"))
      {
        openBrowser("https://github.com/adriankiezik/3dengine");
      }
      ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
  }

  renderConsole();
  renderScripts();
  renderHierarchy();
  renderDiagnostics();
}

void Editor::renderConsole()
{
  if (!showConsole)
    return;

  ImGui::Begin("Console", &showConsole);

  ImGui::Text("This is the console window");

  ImGui::Text("Log 1: Some output...");
  ImGui::Text("Log 2: Another output...");

  ImGui::End();
}

void Editor::renderHierarchy()
{
  if (!showHierarchy)
    return;

  ImGui::Begin("Hierarchy", &showHierarchy);

  ImGui::End();
}

void Editor::renderDiagnostics()
{
  if (!showDiagnostics)
    return;

  ImGui::Begin("Diagnostics", &showDiagnostics);

  ImGui::End();
}

void Editor::renderScripts()
{
  if (!showScripts)
    return;

  ImGui::Begin("Scripts", &showDiagnostics);

  ImGui::End();
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

void Editor::openBrowser(const char *url)
{
#ifdef _WIN32
  ShellExecute(0, "open", url, 0, 0, SW_SHOWNORMAL);
#elif __APPLE__
  std::string command = "open ";
  command += url;
  system(command.c_str());
#elif __linux__
  std::string command = "xdg-open ";
  command += url;
  system(command.c_str());
#else
  std::cerr << "Platform not supported!" << std::endl;
#endif
}

void Editor::styleImGuiComponents(ImVec3 color_for_text, ImVec3 color_for_head, ImVec3 color_for_area, ImVec3 color_for_body, ImVec3 color_for_pops)
{
  ImGuiStyle &style = ImGui::GetStyle();

  style.Colors[ImGuiCol_Text] = ImVec4(color_for_text.x, color_for_text.y, color_for_text.z, 1.00f);
  style.Colors[ImGuiCol_TextDisabled] = ImVec4(color_for_text.x, color_for_text.y, color_for_text.z, 0.58f);
  style.Colors[ImGuiCol_WindowBg] = ImVec4(color_for_body.x, color_for_body.y, color_for_body.z, 0.95f);
  // style.Colors[ImGuiCol_ChildWindowBg] = ImVec4( color_for_area.x, color_for_area.y, color_for_area.z, 0.58f );
  style.Colors[ImGuiCol_Border] = ImVec4(color_for_body.x, color_for_body.y, color_for_body.z, 0.00f);
  style.Colors[ImGuiCol_BorderShadow] = ImVec4(color_for_body.x, color_for_body.y, color_for_body.z, 0.00f);
  style.Colors[ImGuiCol_FrameBg] = ImVec4(color_for_area.x, color_for_area.y, color_for_area.z, 1.00f);
  style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.78f);
  style.Colors[ImGuiCol_FrameBgActive] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
  style.Colors[ImGuiCol_TitleBg] = ImVec4(color_for_area.x, color_for_area.y, color_for_area.z, 1.00f);
  style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(color_for_area.x, color_for_area.y, color_for_area.z, 0.75f);
  style.Colors[ImGuiCol_TitleBgActive] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
  style.Colors[ImGuiCol_MenuBarBg] = ImVec4(color_for_area.x, color_for_area.y, color_for_area.z, 0.47f);
  style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(color_for_area.x, color_for_area.y, color_for_area.z, 1.00f);
  style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.21f);
  style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.78f);
  style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
  // style.Colors[ImGuiCol_ComboBg] = ImVec4( color_for_area.x, color_for_area.y, color_for_area.z, 1.00f );
  style.Colors[ImGuiCol_CheckMark] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.80f);
  style.Colors[ImGuiCol_SliderGrab] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.50f);
  style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
  style.Colors[ImGuiCol_Button] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.50f);
  style.Colors[ImGuiCol_ButtonHovered] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.86f);
  style.Colors[ImGuiCol_ButtonActive] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
  style.Colors[ImGuiCol_Header] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.76f);
  style.Colors[ImGuiCol_HeaderHovered] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.86f);
  style.Colors[ImGuiCol_HeaderActive] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
  // style.Colors[ImGuiCol_Column] = ImVec4( color_for_head.x, color_for_head.y, color_for_head.z, 0.32f );
  // style.Colors[ImGuiCol_ColumnHovered] = ImVec4( color_for_head.x, color_for_head.y, color_for_head.z, 0.78f );
  // style.Colors[ImGuiCol_ColumnActive] = ImVec4( color_for_head.x, color_for_head.y, color_for_head.z, 1.00f );
  style.Colors[ImGuiCol_ResizeGrip] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.15f);
  style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.78f);
  style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
  // style.Colors[ImGuiCol_CloseButton] = ImVec4( color_for_text.x, color_for_text.y, color_for_text.z, 0.16f );
  // style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4( color_for_text.x, color_for_text.y, color_for_text.z, 0.39f );
  // style.Colors[ImGuiCol_CloseButtonActive] = ImVec4( color_for_text.x, color_for_text.y, color_for_text.z, 1.00f );
  style.Colors[ImGuiCol_PlotLines] = ImVec4(color_for_text.x, color_for_text.y, color_for_text.z, 0.63f);
  style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
  style.Colors[ImGuiCol_PlotHistogram] = ImVec4(color_for_text.x, color_for_text.y, color_for_text.z, 0.63f);
  style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
  style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.43f);
  style.Colors[ImGuiCol_PopupBg] = ImVec4(color_for_pops.x, color_for_pops.y, color_for_pops.z, 0.92f);
  // style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4( color_for_area.x, color_for_area.y, color_for_area.z, 0.73f );
}

void Editor::setupImGuiStyle()
{
  static ImVec3 color_for_text = ImVec3(236.f / 255.f, 240.f / 255.f, 241.f / 255.f);
  static ImVec3 color_for_head = ImVec3(41.f / 255.f, 128.f / 255.f, 185.f / 255.f);
  static ImVec3 color_for_area = ImVec3(57.f / 255.f, 79.f / 255.f, 105.f / 255.f);
  static ImVec3 color_for_body = ImVec3(44.f / 255.f, 62.f / 255.f, 80.f / 255.f);
  static ImVec3 color_for_pops = ImVec3(33.f / 255.f, 46.f / 255.f, 60.f / 255.f);

  styleImGuiComponents(color_for_text, color_for_head, color_for_area, color_for_body, color_for_pops);
}