#include "editor.h"
#include <iostream>
#include <sstream>

Editor::Editor(Window &window) : m_window(window) {}

Editor::~Editor() { 
    if (m_initialized) {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    m_initialized = false;
  }
}

bool Editor::initialize() {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();

  ImGui::StyleColorsDark();

  ImGui_ImplGlfw_InitForOpenGL(m_window.getWindow(), true);
  ImGui_ImplOpenGL3_Init("#version 330");

  m_initialized = true;

  return true;
}

void Editor::update() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  renderMainMenu();
  renderFPSMenu();

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Editor::renderMainMenu() {
  ImGui::SetNextWindowPos(ImVec2(25, 25), ImGuiCond_Once);
  ImGui::SetNextWindowCollapsed(true, ImGuiCond_Once);
  ImGui::Begin("3dengine");
  renderWireframeToggle();
  renderVsyncToggle();
  ImGui::End();
}

void Editor::renderFPSMenu() {
  ImGui::SetNextWindowPos(ImVec2(m_window.getWidth() - 150, 25), ImGuiCond_Once);
  ImGui::SetNextWindowCollapsed(false, ImGuiCond_Once);
  ImGui::SetNextWindowSize(ImVec2(125, 50));
  ImGui::Begin("FPS Counter");
  int currentFps = m_window.getFramesPerSecond();
  std::ostringstream oss;
  oss << currentFps;
  std::string numberStr = oss.str();
  std::string result = "FPS: " + numberStr;
  ImGui::Text("%s", result.c_str());
  ImGui::End();
}

void Editor::renderWireframeToggle() {
  if (m_wireframe) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  } else {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }
  ImGui::Checkbox("Toggle Wireframe", &m_wireframe);
}

void Editor::renderVsyncToggle() {
  glfwSwapInterval(m_vsync);
  ImGui::Checkbox("Toggle Vertical Sync", &m_vsync);
}