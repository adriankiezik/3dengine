#include <GLFW/glfw3.h>

#include "process_input.h"

bool holdingButton = false;

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }

  if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS && !holdingButton) {
    GLint polygonMode[2];
    glGetIntegerv(GL_POLYGON_MODE, polygonMode);

    if (polygonMode[0] == GL_FILL) {
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    if (polygonMode[0] == GL_LINE) {
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    holdingButton = true;
  }

  if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_RELEASE) {
    holdingButton = false;
  }
}