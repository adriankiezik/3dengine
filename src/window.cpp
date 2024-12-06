#include "window.h"

Window::Window(const std::string &title, int width, int height)
    : m_width(width), m_height(height), m_title(title.c_str()),
      m_window(nullptr), m_renderedFrames(0), m_lastTime(0), m_framesPerSecond(0) {}

bool Window::initialize() {
  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW" << std::endl;

    return false;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  m_window = glfwCreateWindow(m_width, m_height, m_title, nullptr, nullptr);
  if (!m_window) {
    std::cerr << "Failed to create GLFW window" << std::endl;
    glfwTerminate();

    return false;
  }

  glfwMakeContextCurrent(m_window);

  glfwSetFramebufferSizeCallback(m_window, framebufferSizeCallback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Failed to initialize GLAD" << std::endl;

    return false;
  }

  glEnable(GL_DEPTH_TEST);

  return true;
}

bool Window::shouldClose() const { return glfwWindowShouldClose(m_window); }

void Window::update() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  float currentTime = glfwGetTime();
  m_renderedFrames++;

  if (currentTime - m_lastTime >= 1.0f) {
    m_framesPerSecond = m_renderedFrames;
    m_renderedFrames = 0;
    m_lastTime = currentTime;
  }
}

void Window::postUpdate() {
  glfwSwapBuffers(m_window);
  glfwPollEvents();
}

void Window::terminate() {
  if (m_window) {
    glfwDestroyWindow(m_window);
    m_window = nullptr;
  }
  glfwTerminate();
}

GLFWwindow *Window::getWindow() const { return m_window; }

int Window::getFramesPerSecond() const { return m_framesPerSecond; }

int Window::getWidth() const { return m_width; }

int Window::getHeight() const { return m_height; }

void Window::framebufferSizeCallback(GLFWwindow *window, int width,
                                     int height) {
  glViewport(0, 0, width, height);

  Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
  if (win) {
    win->m_width = width;
    win->m_height = height;
  }
}

Window::~Window() { terminate(); }
