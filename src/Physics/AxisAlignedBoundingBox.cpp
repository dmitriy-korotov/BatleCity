#include "AxisAlignedBoundingBox.h"

namespace Physics
{
	AxisAlignedBoundingBox::AxisAlignedBoundingBox(const glm::vec2& _left_bottom, const glm::vec2& _right_top)
		: left_bottom(_left_bottom)
		, right_top(_right_top)
	{ }
}