#pragma once
#ifndef AXIS_ALIGNED_BOUNDING_BOX
#define AXIS_ALIGNED_BOUNDING_BOX

#include <glm/vec2.hpp>

namespace Physics
{
	struct AxisAlignedBoundingBox
	{
	public:

		AxisAlignedBoundingBox() = delete;
		AxisAlignedBoundingBox(const glm::vec2& _left_bottom, const glm::vec2& _right_top);

		void setAABB(const glm::vec2& _left_bottom, const glm::vec2& _right_top) noexcept;
		void setLeftBottomPoint(const glm::vec2& _left_bottom) noexcept;
		void setRightTopPoint(const glm::vec2& _right_top) noexcept;
		inline void disable() noexcept { m_is_active = false; }
		inline const glm::vec2& getLeftBottom() const noexcept { return m_left_bottom; }
		inline const glm::vec2& getRightTop() const noexcept { return m_right_top; }
		inline bool isActive() const noexcept { return m_is_active; }

	private:

		bool m_is_active = true;
		glm::vec2 m_left_bottom;
		glm::vec2 m_right_top;

	};
	typedef AxisAlignedBoundingBox AABB;
}

#endif // !AXIS_ALIGNED_BOUNDING_BOX
