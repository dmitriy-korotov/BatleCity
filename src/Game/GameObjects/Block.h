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

		virtual void update(const double delta) override;
		virtual void render() const override;

	protected:
		
		enum class ESubBlockLocation : uint8_t
		{
			LeftTop,
			RightTop,
			LeftBottom,
			RightBottom
		};

		enum class ESubBlockState : uint8_t
		{
			Enable,
			Destroy
		};

		virtual void renderBlock(const ESubBlockLocation block_type) const;
	


		EBlockType m_block_type;

	};
}

#endif // BLOCK_H