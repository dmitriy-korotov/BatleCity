#include "Timer.h"

namespace my_system
{
	Timer::Timer()
	{ }

	void Timer::start(const double duration)
	{
		m_is_active = true;
		m_duration = duration;
	}

	void Timer::update(const double delta)
	{
		if (m_is_active)
		{
			m_duration -= delta;
			if (m_duration <= 0)
			{
				m_call_back_function();
			}
		}
	}

	void Timer::setCallBack(const std::function<void()>& function)
	{
		m_call_back_function = function;
	}



	void Timer::reset() noexcept
	{
		m_is_active = false;
		m_duration = 0;
	}
}