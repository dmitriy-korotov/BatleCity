#pragma once
#ifndef WINDOW_H
#define WINDOW_H

#include <GLFW/glfw3.h>

#include <functional>
#include <string>

namespace System {
class Window {
 public:
  Window(const Window&) = delete;
  Window& operator=(const Window&) = delete;

  Window(unsigned int _width, unsigned int _height, const std::string& _name);
  Window(Window&& _other) noexcept;
  Window& operator=(Window&& _right) noexcept;
  ~Window();

  static bool InitWindows() noexcept;
  static void Terminate() noexcept;

  void SetResizeCallBack(void (*_resize_call_back)(GLFWwindow*, int,
                                                   int)) noexcept;
  void SetKeyCallBack(void (*_key_call_back)(GLFWwindow*, int, int, int,
                                             int)) noexcept;
  void CallResizeCallBack(int width, int height) const noexcept;
  void CallKeyCallBack(int key, int scancode, int action,
                       int mode) const noexcept;

  unsigned int GetWindowWidth() const noexcept;
  unsigned int GetWindowHeight() const noexcept;

  bool IsCreated() const noexcept;
  void MakeContextCurrent() const noexcept;
  void SetSize(unsigned int _width, unsigned int _height) const noexcept;
  bool ShouldClose() const noexcept;
  void SwapBuffers() const noexcept;
  void PollEvents() const noexcept;

 private:
  GLFWwindow* m_window_ptr = nullptr;

  std::function<void(GLFWwindow*, int, int)> m_window_resize_call_back;
  std::function<void(GLFWwindow*, int, int, int, int)> m_key_call_back;
};
}  // namespace System

#endif  // !WINDOW_H
