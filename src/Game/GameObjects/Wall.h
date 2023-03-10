#pragma once
#ifndef WALL_H
#define WALL_H

#include "IGameObject.h"

#include "../../Render/Sprite2D.h"

namespace BatleCity
{
	class Wall : public IGameObject
	{
	public:

		enum class EWallType
		{
			All,
			Top,
			Right,
			Bottom,
			Left,
			LeftTop,
			LeftBottom,
			RightBottom,
			RightTop,
			LeftTop_RightBottom,
			RightTop_LeftBottom,
			WithoutLeftTop,
			WithoutLeftBottom,
			WithoutRightBottom,
			WithoutRightTop,
			Nothing
		};

		enum class EBlockLocation
		{
			LeftTop,
			RightTop,
			LeftBottom,
			RightBottom
		};

		enum class EBlockState
		{
			Enable,
			Destroy
		};

		Wall(const EWallType wall_type, const glm::vec2& position, const glm::vec2& size, const float rotation);

		void update(const uint64_t delta) override;
		virtual void renderBlock(const EBlockLocation block_type) const;
		void render() const override;

	protected:
	
		EWallType m_brick_type;

	};
}

#endif // WALL_H