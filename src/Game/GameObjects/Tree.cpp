#include "Tree.h"

#include "../../Resources/ResourceManager.h"

namespace BatleCity
{
	Tree::Tree(const EBlockType wall_type, const glm::vec2& position, const glm::vec2& size, const float rotation)
		: Block(wall_type, position, size, rotation),
		m_sprite(Resources::ResourceManager::getSprite("Tree_8x8"))
	{ }

	void Tree::renderBlock(const ESubBlockLocation subBlock_location) const
	{
		switch (subBlock_location)
		{
		case ESubBlockLocation::LeftTop:
			m_sprite->render(m_position + glm::vec2(0.f, m_size.y / 2), m_size / 2.f, m_rotation);
			break;
		case ESubBlockLocation::RightBottom:
			m_sprite->render(m_position + glm::vec2(m_size.x / 2, 0.f), m_size / 2.f, m_rotation);
			break;
		case ESubBlockLocation::RightTop:
			m_sprite->render(m_position + m_size / 2.f, m_size / 2.f, m_rotation);
			break;
		case ESubBlockLocation::LeftBottom:
			m_sprite->render(m_position, m_size / 2.f, m_rotation);
			break;
		default:
			break;
		}
	}
}