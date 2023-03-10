#include "Wall.h"

#include "../../Render/Sprite2D.h"

#include "../../Resources/ResourceManager.h"

namespace BatleCity
{
	Wall::Wall(const EWallType brick_type, const glm::vec2& position,
						 const glm::vec2& size, const float rotation) : 
																		IGameObject(position, size, rotation),
																		m_brick_type(brick_type)
	{ }



	void Wall::renderBlock(const EBlockLocation block_location) const
	{
		
	}



	void Wall::render() const
	{
		switch (m_brick_type)
		{
		case BatleCity::Wall::EWallType::All:
			renderBlock(EBlockLocation::LeftBottom);
			renderBlock(EBlockLocation::LeftTop);
			renderBlock(EBlockLocation::RightTop);
			renderBlock(EBlockLocation::RightBottom);
			break;
		case BatleCity::Wall::EWallType::Top:
			renderBlock(EBlockLocation::RightTop);
			renderBlock(EBlockLocation::LeftTop);
			break;
		case BatleCity::Wall::EWallType::Right:
			renderBlock(EBlockLocation::RightTop);
			renderBlock(EBlockLocation::RightBottom);
			break;
		case BatleCity::Wall::EWallType::Bottom:
			renderBlock(EBlockLocation::LeftBottom);
			renderBlock(EBlockLocation::RightBottom);
			break;
		case BatleCity::Wall::EWallType::Left:
			renderBlock(EBlockLocation::LeftBottom);
			renderBlock(EBlockLocation::LeftTop);
			break;
		case BatleCity::Wall::EWallType::LeftTop:
			renderBlock(EBlockLocation::LeftTop);
			break;
		case BatleCity::Wall::EWallType::LeftBottom:
			renderBlock(EBlockLocation::LeftBottom);
			break;
		case BatleCity::Wall::EWallType::RightBottom:
			renderBlock(EBlockLocation::RightBottom);
			break;
		case BatleCity::Wall::EWallType::RightTop:
			renderBlock(EBlockLocation::RightTop);
			break;
		case BatleCity::Wall::EWallType::LeftTop_RightBottom:
			renderBlock(EBlockLocation::LeftTop);
			renderBlock(EBlockLocation::RightBottom);
			break;
		case BatleCity::Wall::EWallType::RightTop_LeftBottom:
			renderBlock(EBlockLocation::RightTop);
			renderBlock(EBlockLocation::LeftBottom);
			break;
		case BatleCity::Wall::EWallType::WithoutLeftTop:
			renderBlock(EBlockLocation::LeftBottom);
			renderBlock(EBlockLocation::RightBottom);
			renderBlock(EBlockLocation::RightTop);
			break;
		case BatleCity::Wall::EWallType::WithoutLeftBottom:
			renderBlock(EBlockLocation::LeftTop);
			renderBlock(EBlockLocation::RightBottom);
			renderBlock(EBlockLocation::RightTop);
			break;
		case BatleCity::Wall::EWallType::WithoutRightBottom:
			renderBlock(EBlockLocation::LeftTop);
			renderBlock(EBlockLocation::LeftBottom);
			renderBlock(EBlockLocation::RightTop);
			break;
		case BatleCity::Wall::EWallType::WithoutRightTop:
			renderBlock(EBlockLocation::LeftTop);
			renderBlock(EBlockLocation::LeftBottom);
			renderBlock(EBlockLocation::RightBottom);
			break;
		case BatleCity::Wall::EWallType::Nothing:
			break;
		}
	}



	void Wall::update(const uint64_t delta)
	{ }
}