#include "input_system.h"
#include <iostream>
#include <imgui.h>

Camera *InputSystem::camera = nullptr;
Window *InputSystem::window = nullptr;
float InputSystem::lastX = 400.0f;
float InputSystem::lastY = 300.0f;
bool InputSystem::firstMouse = true;
bool InputSystem::isRightMousePressed = false;

InputSystem::InputSystem(Camera *camera, Window *window)
{
  this->camera = camera;
  this->window = window;

  setupCallbacks();
  configureImGuiMouseHandling();
}

void InputSystem::update()
{
  if (camera && window)
  {
    bool rightMouseDown = glfwGetMouseButton(window->getWindow(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;

    if (rightMouseDown != isRightMousePressed)
    {
      isRightMousePressed = rightMouseDown;
      toggleCameraInputHandling();
    }

    if (isRightMousePressed)
    {
      camera->processKeyboardInput(window->getWindow(), window->getDeltaTime());
    }
  }
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

  if (isRightMousePressed)
  {
    camera->processMouseInput(xOffset, yOffset);
  }
}

void InputSystem::toggleCameraInputHandling()
{
  camera->toggleMouseInput();
  camera->toggleKeyboardInput();
  glfwSetInputMode(window->getWindow(), GLFW_CURSOR,
                   isRightMousePressed ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
  configureImGuiMouseHandling();
}

void InputSystem::configureImGuiMouseHandling()
{
  ImGuiIO &io = ImGui::GetIO();
  bool disableMouse = !isRightMousePressed;
  io.ConfigFlags = disableMouse ? io.ConfigFlags & ~ImGuiConfigFlags_NoMouse : io.ConfigFlags | ImGuiConfigFlags_NoMouse;
}

void InputSystem::setupCallbacks()
{
  glfwSetCursorPosCallback(window->getWindow(), mouseCallback);
  glfwSetInputMode(window->getWindow(), GLFW_CURSOR,
                   isRightMousePressed ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}
