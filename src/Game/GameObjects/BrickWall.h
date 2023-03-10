#pragma once
#ifndef BRICK_WALL_H
#define BRICK_WALL_H

#include <memory>

#include "IGameObject.h"

#include "../../Render/Sprite2D.h"

namespace BatleCity
{
	class BrickWall : public IGameObject
	{
	public:

		enum class EBrickWallType
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

		BrickWall(const EBrickWallType brick_type, const glm::vec2& position, const glm::vec2& size, const float rotation);

		void update(const uint64_t delta) override;
		void render() const override;

	private:
	
		EBrickWallType m_brick_type;
		std::shared_ptr<RenderEngine::Sprite2D> m_sprite;

	};
}

#endif // BRICK_WALL_H