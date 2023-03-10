#include "BetonWall.h"

#include "../../Resources/ResourceManager.h"

namespace BatleCity
{
	BetonWall::BetonWall(const EWallType wall_type, const glm::vec2& position, const glm::vec2& size, const float rotation)
		: Wall(wall_type, position, size, rotation)
	{
		m_sprite = Resources::ResourceManager::getSprite("BetonWall_8x8");
	}

	void BetonWall::renderBlock(const EBlockLocation block_location) const
	{
		switch (block_location)
		{
		case EBlockLocation::LeftTop:
			m_sprite->render(m_position + glm::vec2(0.f, m_size.y / 2), glm::vec2(m_size.x / 2, m_size.y / 2), m_rotation);
			break;
		case EBlockLocation::RightBottom:
			m_sprite->render(m_position + glm::vec2(m_size.x / 2, 0.f), glm::vec2(m_size.x / 2, m_size.y / 2), m_rotation);
			break;
		case EBlockLocation::RightTop:
			m_sprite->render(m_position + glm::vec2(m_size.x / 2, m_size.y / 2), glm::vec2(m_size.x / 2, m_size.y / 2), m_rotation);
			break;
		case EBlockLocation::LeftBottom:
			m_sprite->render(m_position, glm::vec2(m_size.x / 2, m_size.y / 2), m_rotation);
			break;
		default:
			break;
		}
	}
}