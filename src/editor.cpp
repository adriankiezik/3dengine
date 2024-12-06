#include "editor.h"
#include <iostream>

Editor::Editor(Window &window) : window(window) {}

Editor::~Editor() { shutdown(); }

bool Editor::initialize() {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();

  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  ImGui::StyleColorsDark();

  ImGui_ImplGlfw_InitForOpenGL(window.getWindow(), true);
  ImGui_ImplOpenGL3_Init("#version 330");

  initialized = true;

  return true;
}

void Editor::shutdown() {
  if (initialized) {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    initialized = false;
  }
}

void Editor::update() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  renderMainMenu();

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Editor::renderMainMenu() {
  ImGui::SetNextWindowPos(ImVec2(25, 25), ImGuiCond_Once);
  ImGui::SetNextWindowCollapsed(true, ImGuiCond_Once);
  ImGui::Begin("3dengine");
  ImGui::End();
}
