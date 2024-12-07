#include "editor.h"
#include <iostream>
#include <sstream>

Editor::Editor(Window &window) : window(window) {}

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

  renderWireframeToggle();
  renderVsyncToggle();

  ImGui::End();
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
