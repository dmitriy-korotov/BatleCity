#include "AxisAlignedBoundingBox.h"

namespace Physics {
AxisAlignedBoundingBox::AxisAlignedBoundingBox(const glm::vec2& _left_bottom,
                                               const glm::vec2& _right_top)
    : m_left_bottom(_left_bottom), m_right_top(_right_top) {}

void AxisAlignedBoundingBox::SetAABB(const glm::vec2& _left_bottom,
                                     const glm::vec2& _right_top) noexcept {
  m_left_bottom = _left_bottom;
  m_right_top = _right_top;
}

void AxisAlignedBoundingBox::SetLeftBottomPoint(
    const glm::vec2& _left_bottom) noexcept {
  m_left_bottom = _left_bottom;
}

void AxisAlignedBoundingBox::SetRightTopPoint(
    const glm::vec2& _right_top) noexcept {
  m_right_top = _right_top;
}
}  // namespace Physics