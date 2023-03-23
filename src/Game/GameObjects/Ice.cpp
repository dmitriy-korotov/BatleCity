#include "Ice.h"

#include "../../Resources/ResourceManager.h"

namespace BatleCity
{
	Ice::Ice(const EBlockType ice_type, const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer)
		: Block(EGameObjectType::Ice, ice_type, position, size, rotation, layer)
		, m_sprite(Resources::ResourceManager::getSprite("Ice_8x8"))
	{ }

	void Ice::renderBlock(const ESubBlockLocation subBlock_location) const
	{
		switch (subBlock_location)
		{
		case ESubBlockLocation::LeftTop:
			m_sprite->render(m_position + glm::vec2(0.f, m_size.y / 2), m_size / 2.f, m_rotation, m_layer);
			break;
		case ESubBlockLocation::RightBottom:
			m_sprite->render(m_position + glm::vec2(m_size.x / 2, 0.f), m_size / 2.f, m_rotation, m_layer);
			break;
		case ESubBlockLocation::RightTop:
			m_sprite->render(m_position + m_size / 2.f, m_size / 2.f, m_rotation, m_layer);
			break;
		case ESubBlockLocation::LeftBottom:
			m_sprite->render(m_position, m_size / 2.f, m_rotation, m_layer);
			break;
		default:
			break;
		}
	}
}
