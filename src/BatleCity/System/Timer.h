#pragma once
#ifndef TIMER_H
#define TIMER_H

#include <functional>

namespace System {
class Timer final {
 public:
  void Start(const double duration);
  void Update(const double delta);
  void SetCallBack(const std::function<void()>& function);

  void Reset() noexcept;

 private:
  std::function<void()> m_call_back_function = []() {};

  bool m_is_Active = false;
  double m_duration = 0;
};
}  // namespace System

#endif  // !TIMER_H
