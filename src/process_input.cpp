#include <GLFW/glfw3.h>

#include "process_input.h"

bool holdingButton = false;

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }

  if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_RELEASE) {
    holdingButton = false;
  }
}