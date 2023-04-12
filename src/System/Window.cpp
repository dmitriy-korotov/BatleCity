#include "Window.h"

namespace my_system
{
	Window::Window(unsigned int _width, unsigned int _height, const std::string& _name)
	{
		m_window_ptr = glfwCreateWindow(_width, _height, _name.c_str(), nullptr, nullptr);
	}



	Window::Window(Window&& _other) noexcept
			: m_window_ptr(_other.m_window_ptr)
	{
		_other.m_window_ptr = nullptr;
	}



	Window& Window::operator=(Window&& _right) noexcept
	{
		if (this != &_right)
		{
			glfwDestroyWindow(m_window_ptr);
			m_window_ptr = _right.m_window_ptr;

			_right.m_window_ptr = nullptr;
		}
		return *this;
	}



	Window::~Window()
	{
		if (m_window_ptr)
		{
			glfwDestroyWindow(m_window_ptr);
		}
	}



	bool Window::initWindows() noexcept
	{
		return glfwInit();
	}



	void Window::setHint(uint64_t _target, uint64_t _value) noexcept
	{
		glfwWindowHint(_target, _value);
	}



	void Window::terminate() noexcept
	{
		glfwTerminate();
	}



	void Window::setResizeCallBack(void(*_size_call_back)(GLFWwindow*, int, int)) const noexcept
	{
		glfwSetWindowSizeCallback(m_window_ptr, reinterpret_cast<GLFWwindowsizefun&>(_size_call_back));
	}



	void Window::setKeyCallBack(void(*_key_call_back)(GLFWwindow*, int, int, int, int)) const noexcept
	{
		glfwSetKeyCallback(m_window_ptr, reinterpret_cast<GLFWkeyfun&>(_key_call_back));
	}



	bool Window::isCreated() const noexcept
	{
		return m_window_ptr;
	}



	void Window::makeContextCurrent() const noexcept
	{
		glfwMakeContextCurrent(m_window_ptr);
	}



	void Window::setSize(unsigned int _width, unsigned int _height) const noexcept
	{
		glfwSetWindowSize(m_window_ptr, _width, _height);
	}



	bool Window::ShouldClose() const noexcept
	{
		return glfwWindowShouldClose(m_window_ptr);
	}



	void Window::swapBuffers() const noexcept
	{
		glfwSwapBuffers(m_window_ptr);
	}



	void Window::pollEvents() const noexcept
	{
		glfwPollEvents();
	}
}