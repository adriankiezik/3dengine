#include "menu_bar.h"
#include <iostream>
#include <cstdlib>
#include <GLFW/glfw3.h>

MenuBar::MenuBar(Window &window) : window(window) {}

void MenuBar::render()
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
        glfwSetWindowShouldClose(window.getWindow(), GLFW_TRUE);
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
        showConsole = !showConsole;
      }
      if (ImGui::MenuItem("Hierarchy"))
      {
        showHierarchy = !showHierarchy;
      }
      if (ImGui::MenuItem("Scripts"))
      {
        showScripts = !showScripts;
      }
      if (ImGui::MenuItem("Diagnostics"))
      {
        showDiagnostics = !showDiagnostics;
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
        openBrowser("https://github.com/adriankiezik/3dengine");
      }
      ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
  }
}

void MenuBar::openBrowser(const char *url)
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
