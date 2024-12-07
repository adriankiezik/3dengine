#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include "camera.h"
#include "window.h"

class InputHandler
{
public:
  InputHandler(Camera *camera, Window &window);
  static void init();
  void update();

private:
  static Camera *camera;
  static Window *window;
  static float lastX;
  static float lastY;
  static bool firstMouse;
  static bool isF1Pressed;

  static void mouseCallback(GLFWwindow *window, double xpos, double ypos);
  static void toggleCameraInputHandling();
  static bool isF1KeyPressed();
  static void configureImGuiMouseHandling();
  static void setupCallbacks();
};

#endif
