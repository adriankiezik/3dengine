#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include "camera.h"
#include "window.h"

class InputHandler {
public:
    InputHandler(Camera* camera, Window &window);
    void update();
    static void initialize();
    static void mouseCallback(GLFWwindow* window, double xpos, double ypos);

private:
    static Camera* s_camera;
    static Window* s_window;
    static float s_lastX;
    static float s_lastY;
    static bool s_firstMouse;
};

#endif
