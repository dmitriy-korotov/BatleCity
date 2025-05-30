#include <BatleCity/System/Timer.h>

namespace System {
void Timer::Start(const double duration) {
  m_is_active = true;
  m_duration = duration;
}

void Timer::Update(const double delta) {
  if (m_is_active) {
    m_duration -= delta;
    if (m_duration <= 0) {
      m_call_back_function();
    }
  }
}

void Timer::SetCallBack(const std::function<void()>& function) {
  m_call_back_function = function;
}

void Timer::Reset() noexcept {
  m_is_active = false;
  m_duration = 0;
}
}  // namespace System