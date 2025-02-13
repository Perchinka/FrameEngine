#include "Window.hpp"
#include "Logger.hpp"
#include <iostream>

Window::Window(int width, int height, const std::string &title)
    : width(width), height(height) {
  if (!glfwInit()) {
    LOG(CRITICAL, "Failed to initialize GLFW");
    exit(-1);
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  handle = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
  if (!handle) {
    LOG(CRITICAL, "Failed to create GLFW window");
    glfwTerminate();
    exit(-1);
  }

  glfwMakeContextCurrent(handle);
  glfwSetFramebufferSizeCallback(handle, framebufferSizeCallback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    LOG(CRITICAL, "Failed to initialize GLAD");
    exit(-1);
  }

  LOG(INFO, "Window successfully created!");
  glViewport(0, 0, width, height);
}

Window::~Window() {
  glfwDestroyWindow(handle);
  glfwTerminate();
}

bool Window::isOpen() const { return !glfwWindowShouldClose(handle); }

void Window::pollEvents() const { glfwPollEvents(); }

void Window::swapBuffers() const { glfwSwapBuffers(handle); }

void Window::framebufferSizeCallback(GLFWwindow *window, int width,
                                     int height) {
  glViewport(0, 0, width, height);

  Window *win = static_cast<Window *>(glfwGetWindowUserPointer(window));
  if (win) {
    win->width = width;
    win->height = height;
  }
}
