#include "Block.h"

#include "../../Render/Sprite2D.h"

#include "../../Resources/ResourceManager.h"

namespace BatleCity
{
	Block::Block(const EBlockType brick_type, const glm::vec2& position,
						 const glm::vec2& size, const float rotation) : 
																		IGameObject(position, size, rotation),
																		m_brick_type(brick_type)
	{ }



	void Block::renderBlock(const ESubBlockLocation block_location) const
	{ }



	void Block::render() const
	{
		switch (m_brick_type)
		{
		case BatleCity::Block::EBlockType::All:
			renderBlock(ESubBlockLocation::LeftBottom);
			renderBlock(ESubBlockLocation::LeftTop);
			renderBlock(ESubBlockLocation::RightTop);
			renderBlock(ESubBlockLocation::RightBottom);
			break;
		case BatleCity::Block::EBlockType::Top:
			renderBlock(ESubBlockLocation::RightTop);
			renderBlock(ESubBlockLocation::LeftTop);
			break;
		case BatleCity::Block::EBlockType::Right:
			renderBlock(ESubBlockLocation::RightTop);
			renderBlock(ESubBlockLocation::RightBottom);
			break;
		case BatleCity::Block::EBlockType::Bottom:
			renderBlock(ESubBlockLocation::LeftBottom);
			renderBlock(ESubBlockLocation::RightBottom);
			break;
		case BatleCity::Block::EBlockType::Left:
			renderBlock(ESubBlockLocation::LeftBottom);
			renderBlock(ESubBlockLocation::LeftTop);
			break;
		case BatleCity::Block::EBlockType::LeftTop:
			renderBlock(ESubBlockLocation::LeftTop);
			break;
		case BatleCity::Block::EBlockType::LeftBottom:
			renderBlock(ESubBlockLocation::LeftBottom);
			break;
		case BatleCity::Block::EBlockType::RightBottom:
			renderBlock(ESubBlockLocation::RightBottom);
			break;
		case BatleCity::Block::EBlockType::RightTop:
			renderBlock(ESubBlockLocation::RightTop);
			break;
		case BatleCity::Block::EBlockType::LeftTop_RightBottom:
			renderBlock(ESubBlockLocation::LeftTop);
			renderBlock(ESubBlockLocation::RightBottom);
			break;
		case BatleCity::Block::EBlockType::RightTop_LeftBottom:
			renderBlock(ESubBlockLocation::RightTop);
			renderBlock(ESubBlockLocation::LeftBottom);
			break;
		case BatleCity::Block::EBlockType::WithoutLeftTop:
			renderBlock(ESubBlockLocation::LeftBottom);
			renderBlock(ESubBlockLocation::RightBottom);
			renderBlock(ESubBlockLocation::RightTop);
			break;
		case BatleCity::Block::EBlockType::WithoutLeftBottom:
			renderBlock(ESubBlockLocation::LeftTop);
			renderBlock(ESubBlockLocation::RightBottom);
			renderBlock(ESubBlockLocation::RightTop);
			break;
		case BatleCity::Block::EBlockType::WithoutRightBottom:
			renderBlock(ESubBlockLocation::LeftTop);
			renderBlock(ESubBlockLocation::LeftBottom);
			renderBlock(ESubBlockLocation::RightTop);
			break;
		case BatleCity::Block::EBlockType::WithoutRightTop:
			renderBlock(ESubBlockLocation::LeftTop);
			renderBlock(ESubBlockLocation::LeftBottom);
			renderBlock(ESubBlockLocation::RightBottom);
			break;
		case BatleCity::Block::EBlockType::Nothing:
			break;
		}
	}



	void Block::update(const uint64_t delta)
	{ }
}