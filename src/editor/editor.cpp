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
      menuBar(window, scene, camera),
      projectSelection(projectCreation, window, camera, menuBar, scene),
      projectCreation(projectSelection, menuBar),
      viewportWindow(framebuffer, camera),
      consoleWindow(console),
      hierarchyWindow(scene),
      scriptsWindow(scene),
      diagnosticsWindow()
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

  // Initialize windows that need ImGui context
  scriptsWindow.init();

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

  if (!projectSelection.isProjectSelected())
  {
    projectSelection.render();
    projectCreation.render();
    endFrame();
    return;
  }

  menuBar.render();
  viewportWindow.render();
  consoleWindow.render(menuBar.showConsole);
  hierarchyWindow.render(menuBar.showHierarchy);
  scriptsWindow.render(menuBar.showScripts);
  diagnosticsWindow.render(menuBar.showDiagnostics);

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
