#include "input_handler.h"
#include <iostream>

Camera* InputHandler::s_camera = nullptr;
Window* InputHandler::s_window = nullptr;
float InputHandler::s_lastX = 400.0f; 
float InputHandler::s_lastY = 300.0f;
bool InputHandler::s_firstMouse = true;

InputHandler::InputHandler(Camera* camera, Window &window) {
  s_camera = camera;
  s_window = &window;
}

void InputHandler::initialize() {
    if (s_window) {
        glfwSetCursorPosCallback(s_window->getWindow(), mouseCallback);
    }
}

void InputHandler::update() {
    if (s_camera && s_window) {
        s_camera->processKeyboardInput(s_window->getWindow(), s_window->getDeltaTime());
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
