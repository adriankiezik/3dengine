#include "input_system.h"
#include <iostream>
#include <imgui.h>

Camera *InputSystem::camera = nullptr;
Window *InputSystem::window = nullptr;
float InputSystem::lastX = 400.0f;
float InputSystem::lastY = 300.0f;
bool InputSystem::firstMouse = true;
bool InputSystem::isF1Pressed = false;

InputSystem::InputSystem(Camera *camera, Window &window)
{
  this->camera = camera;
  this->window = &window;
}

void InputSystem::init()
{
  if (window)
  {
    setupCallbacks();
    configureImGuiMouseHandling();
  }
}

void InputSystem::update()
{
  if (camera && window)
  {
    camera->processKeyboardInput(window->getWindow(), window->getDeltaTime());
  }

  if (isF1KeyPressed())
  {
    toggleCameraInputHandling();
  }
}

bool InputSystem::isF1KeyPressed()
{
  if (glfwGetKey(window->getWindow(), GLFW_KEY_F1) == GLFW_PRESS)
  {
    if (!isF1Pressed)
    {
      isF1Pressed = true;
      return true;
    }
  }
  else
  {
    isF1Pressed = false;
  }
  return false;
}

void InputSystem::mouseCallback(GLFWwindow *window, double xpos, double ypos)
{
  if (firstMouse)
  {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  float xOffset = xpos - lastX;
  float yOffset = lastY - ypos;
  lastX = xpos;
  lastY = ypos;

  if (camera)
  {
    camera->processMouseInput(xOffset, yOffset);
  }
}

void InputSystem::toggleCameraInputHandling()
{
  camera->toggleMouseInput();
  camera->toggleKeyboardInput();
  glfwSetInputMode(window->getWindow(), GLFW_CURSOR,
                   camera->isKeyboardInputEnabled() ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
  configureImGuiMouseHandling();
}

void InputSystem::configureImGuiMouseHandling()
{
  ImGuiIO &io = ImGui::GetIO();
  bool disableMouse = !camera->isKeyboardInputEnabled();
  io.ConfigFlags = disableMouse ? io.ConfigFlags & ~ImGuiConfigFlags_NoMouse : io.ConfigFlags | ImGuiConfigFlags_NoMouse;
}

void InputSystem::setupCallbacks()
{
  glfwSetCursorPosCallback(window->getWindow(), mouseCallback);
  glfwSetInputMode(window->getWindow(), GLFW_CURSOR,
                   camera->isKeyboardInputEnabled() ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}
