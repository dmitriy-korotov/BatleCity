#pragma once
#ifndef WINDOW_H
#define WINDOW_H

#include <GLFW/glfw3.h>

#include <iostream>



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

		void setResizeCallBack(void(*_size_call_back)(GLFWwindow*, int, int)) const noexcept;
		void setKeyCallBack(void(*_key_call_back)(GLFWwindow*, int, int, int, int)) const noexcept;

		bool isCreated() const noexcept;
		void makeContextCurrent() const noexcept;
		void setSize(unsigned int _width, unsigned int _height) const noexcept;
		bool ShouldClose() const noexcept;
		void swapBuffers() const noexcept;
		void pollEvents() const noexcept;

	private:

		GLFWwindow* m_window_ptr = nullptr;

	};
}

#endif // !WINDOW_H
