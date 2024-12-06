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
  void postUpdate();
  void terminate();

  GLFWwindow *getWindow() const;
  int getWidth() const;
  int getHeight() const;
  int getFramesPerSecond() const;
  float getDeltaTime() const;

  ~Window();

private:
  int m_width;
  int m_height;
  const char *m_title;
  GLFWwindow *m_window;

  int m_renderedFrames;
  int m_framesPerSecond;
  float m_lastTime;
  float m_lastFrame;
  float m_deltaTime;

  void calculateDeltaTime();
  static void framebufferSizeCallback(GLFWwindow *window, int width,
                                      int height);
};

#endif
