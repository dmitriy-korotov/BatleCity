#pragma once
#ifndef TIMER_H
#define TIMER_H

#include <functional>
#include <iostream>

namespace my_system {
class Timer {
 public:
  Timer();

  void start(const double duration);
  void update(const double delta);
  void setCallBack(const std::function<void()>& function);

  void reset() noexcept;

 private:
  std::function<void()> m_call_back_function = []() {};

  bool m_is_active = false;
  double m_duration = 0;
};
}  // namespace my_system

#endif  // !TIMER_H
