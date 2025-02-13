#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <string>

class Window {
private:
  GLFWwindow *handle;
  int width, height;

public:
  Window(int width, int height, const std::string &title);
  ~Window();

  bool isOpen() const;
  void pollEvents() const;
  void swapBuffers() const;

  int getWidth() const { return width; }
  int getHeight() const { return height; }

  static void framebufferSizeCallback(GLFWwindow *window, int width,
                                      int height);
};

#endif // WINDOW_HPP
