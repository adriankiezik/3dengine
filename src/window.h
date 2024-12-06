#ifndef WINDOW_H
#define WINDOW_H

#include <glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

class Window {
public:
  Window(const std::string &title, int width, int height);

  bool initialize();
  bool shouldClose() const;
  void update();
  void terminate();

  GLFWwindow *getWindow() const;

  ~Window();

private:
  int m_width;
  int m_height;
  const char *m_title;
  GLFWwindow *m_window;

  static void framebufferSizeCallback(GLFWwindow *window, int width,
                                      int height);
};

#endif
