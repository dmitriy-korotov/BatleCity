#include "Border.h"

#include "../../Resources/ResourceManager.h"

namespace BatleCity
{
	Border::Border(const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer)
		: Block(EGameObjectType::Border, EBlockType::All, position, size, rotation, layer)
		, m_sprite(Resources::ResourceManager::getSprite("Border_8x8"))
	{
		m_colliders.emplace_back(glm::vec2(0.f), m_size);
	}

	void Border::render() const
	{
		m_sprite->render(m_position, m_size, m_rotation, m_layer);
	}
}
