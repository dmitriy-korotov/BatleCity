#pragma once
#ifndef AXIS_ALIGNED_BOUNDING_BOX
#define AXIS_ALIGNED_BOUNDING_BOX

#include <glm/vec2.hpp>

namespace Physics {
struct AxisAlignedBoundingBox {
 public:
  AxisAlignedBoundingBox() = delete;
  AxisAlignedBoundingBox(const glm::vec2& _left_bottom,
                         const glm::vec2& _right_top);

  void SetAABB(const glm::vec2& _left_bottom,
               const glm::vec2& _right_top) noexcept;
  void SetLeftBottomPoint(const glm::vec2& _left_bottom) noexcept;
  void SetRightTopPoint(const glm::vec2& _right_top) noexcept;
  inline void Disable() noexcept { m_is_Active = false; }
  inline const glm::vec2& getLeftBottom() const noexcept {
    return m_left_bottom;
  }
  inline const glm::vec2& getRightTop() const noexcept { return m_right_top; }
  inline bool IsActive() const noexcept { return m_is_Active; }

 private:
  bool m_is_Active = true;
  glm::vec2 m_left_bottom;
  glm::vec2 m_right_top;
};
typedef AxisAlignedBoundingBox AABB;
}  // namespace Physics

#endif  // !AXIS_ALIGNED_BOUNDING_BOX
