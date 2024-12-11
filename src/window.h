#ifndef WINDOW_H
#define WINDOW_H

#include <glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include "framebuffer.h"

class Window
{
public:
  Window(const std::string &title, int width, int height);

  void update();
  void postUpdate();
  bool shouldClose() const;
  void terminate();

  GLFWwindow *getWindow() const;
  Framebuffer &getFramebuffer();
  int getWidth() const;
  int getHeight() const;
  int getFramesPerSecond() const;
  float getDeltaTime() const;

  ~Window();

private:
  int width;
  int height;
  const char *title;

  GLFWwindow *window;
  Framebuffer framebuffer;

  int renderedFrames;
  int framesPerSecond;
  float lastTime;
  float lastFrame;
  float deltaTime;

  bool init();
  void calculateDeltaTime();
  static void framebufferSizeCallback(GLFWwindow *window, int width,
                                      int height);
  void updateMousePosition();
};

#endif
