#include "Window.h"
#include "GLFW/glfw3.h"

#include <iostream>

namespace System {
Window::Window(unsigned int _width, unsigned int _height,
               const std::string& _name) {
  m_window_ptr =
      glfwCreateWindow(_width, _height, _name.c_str(), nullptr, nullptr);
  if (!m_window_ptr) {
    const char* message;
    glfwGetError(&message);
    std::cerr << "ERROR: " << message << std::endl;
  }
  glfwSetWindowPos(m_window_ptr, 0, 0);
}

Window::Window(Window&& _other) noexcept : m_window_ptr(_other.m_window_ptr) {
  _other.m_window_ptr = nullptr;
}

Window& Window::operator=(Window&& _right) noexcept {
  if (this != &_right) {
    glfwDestroyWindow(m_window_ptr);
    m_window_ptr = _right.m_window_ptr;

    _right.m_window_ptr = nullptr;
  }
  return *this;
}

Window::~Window() {
  if (m_window_ptr) {
    glfwDestroyWindow(m_window_ptr);
  }
}

bool Window::InitWindows() noexcept { return glfwInit(); }

void Window::Terminate() noexcept { glfwTerminate(); }

void Window::SetResizeCallBack(void (*_resize_call_back)(GLFWwindow*, int,
                                                         int)) noexcept {
  m_window_resize_call_back = _resize_call_back;
  glfwSetWindowSizeCallback(
      m_window_ptr, reinterpret_cast<GLFWwindowsizefun&>(_resize_call_back));
}

void Window::SetKeyCallBack(void (*_key_call_back)(GLFWwindow*, int, int, int,
                                                   int)) noexcept {
  m_key_call_back = _key_call_back;
  glfwSetKeyCallback(m_window_ptr,
                     reinterpret_cast<GLFWkeyfun&>(_key_call_back));
}

void Window::CallResizeCallBack(int width, int height) const noexcept {
  m_window_resize_call_back(m_window_ptr, width, height);
}

void Window::CallKeyCallBack(int key, int scancode, int action,
                             int mode) const noexcept {
  m_key_call_back(m_window_ptr, key, scancode, action, mode);
}

unsigned int Window::GetWindowWidth() const noexcept {
  int width = 0, height = 0;
  glfwGetWindowSize(m_window_ptr, &width, &height);
  return width;
}

unsigned int Window::GetWindowHeight() const noexcept {
  int width = 0, height = 0;
  glfwGetWindowSize(m_window_ptr, &width, &height);
  return height;
}

bool Window::IsCreated() const noexcept { return m_window_ptr; }

void Window::MakeContextCurrent() const noexcept {
  glfwMakeContextCurrent(m_window_ptr);
}

void Window::SetSize(unsigned int _width, unsigned int _height) const noexcept {
  glfwSetWindowSize(m_window_ptr, _width, _height);
}

bool Window::ShouldClose() const noexcept {
  return glfwWindowShouldClose(m_window_ptr);
}

void Window::SwapBuffers() const noexcept { glfwSwapBuffers(m_window_ptr); }

void Window::PollEvents() const noexcept { glfwPollEvents(); }
}  // namespace System