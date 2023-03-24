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

		inline const glm::vec2& getLeftBottom() const noexcept { return left_bottom; }
		inline const glm::vec2& getRightTop() const noexcept { return right_top; }

	private:

		glm::vec2 left_bottom;
		glm::vec2 right_top;

	};
	typedef AxisAlignedBoundingBox AABB;
}

#endif // !AXIS_ALIGNED_BOUNDING_BOX
