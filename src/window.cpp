#include "window.h"
#include <imgui.h>
#include "framebuffer.h"

Window::Window(const std::string &title, int width, int height)
    : width(width), height(height), title(title.c_str()),
      window(nullptr), renderedFrames(0), lastTime(0), framesPerSecond(0)
{
  init();
}

bool Window::init()
{
  if (!glfwInit())
  {
    std::cerr << "Failed to initialize GLFW" << std::endl;

    return false;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  window = glfwCreateWindow(width, height, title, nullptr, nullptr);
  if (!window)
  {
    std::cerr << "Failed to create GLFW window" << std::endl;
    glfwTerminate();

    return false;
  }

  glfwMakeContextCurrent(window);

  glfwSetWindowUserPointer(window, this);

  glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cerr << "Failed to initialize GLAD" << std::endl;

    return false;
  }

  glEnable(GL_DEPTH_TEST);

  framebuffer.init();

  return true;
}

bool Window::shouldClose() const { return glfwWindowShouldClose(window); }

void Window::update()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(0.678f, 0.847f, 1.0f, 1.0f);

  float currentTime = glfwGetTime();
  renderedFrames++;

  updateMousePosition();
  calculateDeltaTime();

  if (currentTime - lastTime >= 1.0f)
  {
    framesPerSecond = renderedFrames;
    renderedFrames = 0;
    lastTime = currentTime;
  }
}

void Window::postUpdate()
{
  glfwSwapBuffers(window);
  glfwPollEvents();
}

void Window::terminate()
{
  if (window)
  {
    glfwDestroyWindow(window);
    window = nullptr;
  }
  glfwTerminate();
}

void Window::calculateDeltaTime()
{
  float currentFrame = glfwGetTime();
  deltaTime = currentFrame - lastFrame;
  lastFrame = currentFrame;
}

GLFWwindow *Window::getWindow() const { return window; }

int Window::getFramesPerSecond() const { return framesPerSecond; }

int Window::getWidth() const { return width; }

int Window::getHeight() const { return height; }

float Window::getDeltaTime() const { return deltaTime; }

Framebuffer &Window::getFramebuffer()
{
  return framebuffer;
}

void Window::framebufferSizeCallback(GLFWwindow *window, int width,
                                     int height)
{
  glViewport(0, 0, width, height);

  Window *win = static_cast<Window *>(glfwGetWindowUserPointer(window));
  if (win)
  {
    win->width = width;
    win->height = height;
  }
}

void Window::updateMousePosition()
{
  double mouseX, mouseY;
  glfwGetCursorPos(window, &mouseX, &mouseY);
  ImGuiIO &io = ImGui::GetIO();
  io.MousePos = ImVec2((float)mouseX, (float)mouseY);
}

Window::~Window() { terminate(); }
