#pragma once
#ifndef AXIS_ALIGNED_BOUNDING_BOX
#define AXIS_ALIGNED_BOUNDING_BOX

#include <glm/vec2.hpp>

namespace Physics
{
	struct AxisAlignedBoundingBox
	{
		AxisAlignedBoundingBox(const glm::vec2& _left_bottom, const glm::vec2& _right_top)
			: left_bottom(_left_bottom)
			, right_top(_right_top)
		{ }

		glm::vec2 left_bottom;
		glm::vec2 right_top;
	};
	typedef AxisAlignedBoundingBox AABB;
}

#endif // !AXIS_ALIGNED_BOUNDING_BOX
