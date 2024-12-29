#ifndef INPUT_SYSTEM_H
#define INPUT_SYSTEM_H

#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include "camera.h"
#include "window.h"

class InputSystem
{
public:
  InputSystem(Camera *camera, Window *window);
  void update();

private:
  static Camera *camera;
  static Window *window;
  static float lastX;
  static float lastY;
  static bool firstMouse;
  static bool isRightMousePressed;

  static void mouseCallback(GLFWwindow *window, double xpos, double ypos);
  static void toggleCameraInputHandling();
  static void configureImGuiMouseHandling();
  static void setupCallbacks();
};

#endif
