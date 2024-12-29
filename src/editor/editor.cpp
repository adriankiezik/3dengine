#include "editor.h"
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

Editor::Editor(Window &window, Scene &scene, Camera &camera,
               ScriptSystem &scriptSystem, Framebuffer &framebuffer,
               ConsoleStreamBuffer &console)
    : window(window),
      scene(scene),
      camera(camera),
      scriptSystem(scriptSystem),
      framebuffer(framebuffer),
      console(console),
      menuBar(window),
      viewportWindow(framebuffer, camera),
      consoleWindow(console),
      hierarchyWindow(scene),
      diagnosticsWindow(),
      scriptsWindow(),
      projectDialogShown(false)
{
  if (!init())
  {
    throw std::runtime_error("Failed to initialize the editor");
  }
}

Editor::~Editor()
{
  if (initialized)
  {
    shutdown();
  }
}

bool Editor::init()
{
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui::StyleColorsDark();

  ImGui_ImplGlfw_InitForOpenGL(window.getWindow(), true);
  ImGui_ImplOpenGL3_Init("#version 330");

  initialized = true;

  return true;
}

void Editor::shutdown()
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  initialized = false;
}

void Editor::update()
{
  startFrame();

  if (!projectDialogShown)
  {
    renderProjectDialog();
  }
  else
  {
    menuBar.render();

    viewportWindow.render();
    consoleWindow.render(menuBar.showConsole);
    hierarchyWindow.render(menuBar.showHierarchy);
    scriptsWindow.render(menuBar.showScripts);
    diagnosticsWindow.render(menuBar.showDiagnostics);
  }

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

void Editor::renderProjectDialog()
{
  ImGui::OpenPopup("Project Selection");

  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));

  if (ImGui::BeginPopupModal("Project Selection", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
  {
    ImGui::Text("Welcome to the 3D Engine!");
    ImGui::Separator();

    if (ImGui::Button("Create New Project", ImVec2(200, 0)))
    {
      // TODO: Implement new project creation
      projectDialogShown = true;
      ImGui::CloseCurrentPopup();
    }

    ImGui::Spacing();

    if (ImGui::Button("Open Existing Project", ImVec2(200, 0)))
    {
      // TODO: Implement project opening
      projectDialogShown = true;
      ImGui::CloseCurrentPopup();
    }

    ImGui::EndPopup();
  }
}
