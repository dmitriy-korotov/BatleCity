#pragma once
#ifndef TIMER_H
#define TIMER_H

#include <iostream>
#include <functional>

namespace my_system
{
	class Timer
	{
	public:

		Timer();

		void start(const double duration);
		void update(const double delta);
		void setCallBack(const std::function<void()>& function);

	private:

		std::function<void()> m_call_back_function;

		bool m_is_active;
		double m_duration;

	};
}

#endif // !TIMER_H
