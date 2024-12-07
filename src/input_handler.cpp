#include "input_handler.h"
#include <iostream>
#include <imgui.h>

Camera *InputHandler::camera = nullptr;
Window *InputHandler::window = nullptr;
float InputHandler::lastX = 400.0f;
float InputHandler::lastY = 300.0f;
bool InputHandler::firstMouse = true;
bool InputHandler::isF1Pressed = false;

InputHandler::InputHandler(Camera *camera, Window &window)
{
  this->camera = camera;
  this->window = &window;
}

void InputHandler::init()
{
  if (window)
  {
    setupCallbacks();
    configureImGuiMouseHandling();
  }
}

void InputHandler::update()
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

bool InputHandler::isF1KeyPressed()
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

void InputHandler::mouseCallback(GLFWwindow *window, double xpos, double ypos)
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

void InputHandler::toggleCameraInputHandling()
{
  camera->toggleMouseInput();
  camera->toggleKeyboardInput();
  glfwSetInputMode(window->getWindow(), GLFW_CURSOR,
                   camera->isKeyboardInputEnabled() ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
  configureImGuiMouseHandling();
}

void InputHandler::configureImGuiMouseHandling()
{
  ImGuiIO &io = ImGui::GetIO();
  bool disableMouse = !camera->isKeyboardInputEnabled();
  io.ConfigFlags = disableMouse ? io.ConfigFlags & ~ImGuiConfigFlags_NoMouse : io.ConfigFlags | ImGuiConfigFlags_NoMouse;
}

void InputHandler::setupCallbacks()
{
  glfwSetCursorPosCallback(window->getWindow(), mouseCallback);
  glfwSetInputMode(window->getWindow(), GLFW_CURSOR,
                   camera->isKeyboardInputEnabled() ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}
