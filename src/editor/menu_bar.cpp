#include "menu_bar.h"
#include <iostream>
#include <cstdlib>
#include <GLFW/glfw3.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <filesystem>

MenuBar::MenuBar(Window &window, Scene &scene, Camera &camera)
    : window(window), scene(scene), camera(camera) {}

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
        saveScene();
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

void MenuBar::saveScene()
{
  if (currentProjectPath.empty())
  {
    std::cerr << "No project is currently open" << std::endl;
    return;
  }

  std::cout << "Current project path: " << currentProjectPath << std::endl;
  std::filesystem::path projectPath(currentProjectPath);
  std::filesystem::path scenesDir = projectPath / "scenes";
  std::filesystem::path sceneFile = scenesDir / "scene.3dscene";

  // Create scenes directory if it doesn't exist
  try
  {
    if (!std::filesystem::exists(scenesDir))
    {
      std::filesystem::create_directories(scenesDir);
      std::cout << "Created scenes directory: " << scenesDir << std::endl;
    }
  }
  catch (const std::filesystem::filesystem_error &e)
  {
    std::cerr << "Failed to create scenes directory: " << e.what() << std::endl;
    return;
  }

  std::cout << "Saving scene to: " << sceneFile << std::endl;
  std::cout << "Scene objects count: " << scene.getObjects().size() << std::endl;

  std::ofstream sceneStream(sceneFile);
  if (!sceneStream.is_open())
  {
    std::cerr << "Failed to open scene file for writing: " << sceneFile << std::endl;
    return;
  }

  try
  {
    nlohmann::ordered_json sceneData;

    sceneData["camera"] = camera.toJson();
    sceneData["scene"] = scene.toJson();

    sceneStream << sceneData.dump(4);
    sceneStream.close();

    std::cout << "Scene saved successfully" << std::endl;
  }
  catch (const std::exception &e)
  {
    std::cerr << "Error while saving scene: " << e.what() << std::endl;
  }
}
