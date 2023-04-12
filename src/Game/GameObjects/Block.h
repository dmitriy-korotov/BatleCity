#pragma once
#ifndef BLOCK_H
#define BLOCK_H

#include "IGameObject.h"

#include "../../Render/Sprite2D.h"

namespace BatleCity
{
	class Block : public IGameObject
	{
	public:

		enum class EBlockType : uint8_t
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

		Block(EGameObjectType game_object_type, const EBlockType wall_type, const glm::vec2& position,
			  const glm::vec2& size, const float rotation, const float layer);

		void update(const double delta) override;
		void render() const override;
		bool onCollision(EGameObjectType game_object_type, std::shared_ptr<Physics::AABB> target_collider, const glm::vec2& direction) override;

	protected:
		
		enum class ESubBlockLocation : uint8_t
		{
			LeftBottom = 0,
			LeftTop,
			RightTop,
			RightBottom
		};

		enum class ESubBlockState : uint8_t
		{
			Enable,
			Destroy
		};

		virtual void renderBlock(const ESubBlockLocation subBlock_location) const;
	


		EBlockType m_block_type;

	};
}

#endif // BLOCK_H