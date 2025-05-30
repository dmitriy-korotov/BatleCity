#include <BatleCity/System/Window.h>

#include <iostream>

namespace System {
Window::Window(unsigned int _width, unsigned int _height,
               const std::string& _name) {
  if (!glfwInit()) {
    const char* message;
    glfwGetError(&message);
    std::cerr << "ERROR: " << message << std::endl;
  }
  m_window = glfwCreateWindow(_width, _height, _name.c_str(), nullptr, nullptr);
  if (!m_window) {
    const char* message;
    glfwGetError(&message);
    std::cerr << "ERROR: " << message << std::endl;
  }
  glfwSetWindowPos(m_window, 0, 0);
}

Window::Window(Window&& _other) noexcept : m_window(_other.m_window) {
  _other.m_window = nullptr;
}

Window& Window::operator=(Window&& _right) noexcept {
  if (this != &_right) {
    glfwDestroyWindow(m_window);
    m_window = _right.m_window;

    _right.m_window = nullptr;
  }
  return *this;
}

Window::~Window() {
  if (m_window) {
    glfwDestroyWindow(m_window);
  }
  glfwTerminate();
}

void Window::SetResizeCallBack(void (*_resize_call_back)(GLFWwindow*, int,
                                                         int)) noexcept {
  m_window_resize_call_back = _resize_call_back;
  glfwSetWindowSizeCallback(
      m_window, reinterpret_cast<GLFWwindowsizefun&>(_resize_call_back));
}

void Window::SetKeyCallBack(void (*_key_call_back)(GLFWwindow*, int, int, int,
                                                   int)) noexcept {
  m_key_call_back = _key_call_back;
  glfwSetKeyCallback(m_window, reinterpret_cast<GLFWkeyfun&>(_key_call_back));
}

void Window::CallResizeCallBack(int width, int height) const noexcept {
  m_window_resize_call_back(m_window, width, height);
}

void Window::CallKeyCallBack(int key, int scancode, int action,
                             int mode) const noexcept {
  m_key_call_back(m_window, key, scancode, action, mode);
}

unsigned int Window::GetWindowWidth() const noexcept {
  int width = 0, height = 0;
  glfwGetWindowSize(m_window, &width, &height);
  return width;
}

unsigned int Window::GetWindowHeight() const noexcept {
  int width = 0, height = 0;
  glfwGetWindowSize(m_window, &width, &height);
  return height;
}

bool Window::IsCreated() const noexcept { return m_window; }

void Window::MakeContextCurrent() const noexcept {
  glfwMakeContextCurrent(m_window);
}

void Window::SetSize(unsigned int _width, unsigned int _height) const noexcept {
  glfwSetWindowSize(m_window, _width, _height);
}

bool Window::ShouldClose() const noexcept {
  return glfwWindowShouldClose(m_window);
}

void Window::SwapBuffers() const noexcept { glfwSwapBuffers(m_window); }

void Window::PollEvents() const noexcept { glfwPollEvents(); }
}  // namespace System