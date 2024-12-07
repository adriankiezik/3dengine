#ifndef WINDOW_H
#define WINDOW_H

#include <glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

class Window
{
public:
  Window(const std::string &title, int width, int height);

  bool init();
  void update();
  void postUpdate();
  bool shouldClose() const;
  void terminate();

  GLFWwindow *getWindow() const;
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

  int renderedFrames;
  int framesPerSecond;
  float lastTime;
  float lastFrame;
  float deltaTime;

  void calculateDeltaTime();
  static void framebufferSizeCallback(GLFWwindow *window, int width,
                                      int height);
  void updateMousePosition();
};

#endif
