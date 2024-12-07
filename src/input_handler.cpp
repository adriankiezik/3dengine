#include "input_handler.h"
#include <iostream>
#include <imgui.h>

Camera* InputHandler::s_camera = nullptr;
Window* InputHandler::s_window = nullptr;
float InputHandler::s_lastX = 400.0f; 
float InputHandler::s_lastY = 300.0f;
bool InputHandler::s_firstMouse = true;

InputHandler::InputHandler(Camera* camera, Window &window): isF1Pressed(false) {
  s_camera = camera;
  s_window = &window;
}

void InputHandler::initialize() {
    if (s_window) {
      glfwSetCursorPosCallback(s_window->getWindow(), mouseCallback);
      glfwSetInputMode(s_window->getWindow(), GLFW_CURSOR, s_camera->isKeyboardInputEnabled() ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
      ImGuiIO& io = ImGui::GetIO();
      io.ConfigFlags = !s_camera->isKeyboardInputEnabled() ? io.ConfigFlags & ~ImGuiConfigFlags_NoMouse : io.ConfigFlags | ImGuiConfigFlags_NoMouse;
    }
}

void InputHandler::update() {
    if (s_camera && s_window) {
        s_camera->processKeyboardInput(s_window->getWindow(), s_window->getDeltaTime());
    }
    if (glfwGetKey(s_window->getWindow(), GLFW_KEY_F1) == GLFW_PRESS) {
      if (!isF1Pressed) {
        toggleCameraInputHandling();
        isF1Pressed = true;
      }
    } else {
      isF1Pressed = false;
    }
}

void InputHandler::mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    if (s_firstMouse) {
        s_lastX = xpos;
        s_lastY = ypos;
        s_firstMouse = false;
    }

    float xOffset = xpos - s_lastX;
    float yOffset = s_lastY - ypos; // Reversed: screen coordinates are top-down
    s_lastX = xpos;
    s_lastY = ypos;

    if (s_camera) {
        s_camera->processMouseInput(xOffset, yOffset);
    }
}

void InputHandler::toggleCameraInputHandling() {
  s_camera->toggleMouseInput();
  s_camera->toggleKeyboardInput();
  glfwSetInputMode(s_window->getWindow(), GLFW_CURSOR, s_camera->isKeyboardInputEnabled() ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags = !s_camera->isKeyboardInputEnabled() ? io.ConfigFlags & ~ImGuiConfigFlags_NoMouse : io.ConfigFlags | ImGuiConfigFlags_NoMouse;
}