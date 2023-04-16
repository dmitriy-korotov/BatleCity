#pragma once
#ifndef WINDOW_H
#define WINDOW_H

#include <GLFW/glfw3.h>

#include <iostream>
#include <functional>


namespace my_system
{
	class Window
	{
	public:

		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

		Window(unsigned int _width, unsigned int _height, const std::string& _name);
		Window(Window&& _other) noexcept;
		Window& operator=(Window&& _right) noexcept;
		~Window();

		static bool initWindows() noexcept;
		static void setHint(uint64_t _target, uint64_t _value) noexcept;
		static void terminate() noexcept;

		void setResizeCallBack(void(*_resize_call_back)(GLFWwindow*, int, int)) noexcept;
		void setKeyCallBack(void(*_key_call_back)(GLFWwindow*, int, int, int, int)) noexcept;
		void callResizeCallBack(int width, int height) const noexcept;
		void callKeyCallBack(int key, int scancode, int action, int mode) const noexcept;

		unsigned int getWindowWidth() const noexcept;
		unsigned int getWindowHeight() const noexcept;

		bool isCreated() const noexcept;
		void makeContextCurrent() const noexcept;
		void setSize(unsigned int _width, unsigned int _height) const noexcept;
		bool ShouldClose() const noexcept;
		void swapBuffers() const noexcept;
		void pollEvents() const noexcept;

	private:

		GLFWwindow* m_window_ptr = nullptr;

		std::function<void(GLFWwindow*, int, int)> m_window_resize_call_back;
		std::function<void(GLFWwindow*, int, int, int, int)> m_key_call_back;

	};
}

#endif // !WINDOW_H
