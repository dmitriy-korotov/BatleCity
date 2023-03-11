#include "Eagle.h"

#include "../../Resources/ResourceManager.h"

namespace BatleCity
{
	Eagle::Eagle(const glm::vec2& position, const glm::vec2& size, const float rotation)
		: IGameObject(position, size, rotation),
		  m_eagle_state(EEagleState::Elive),
		  m_eagle_sprites{ Resources::ResourceManager::getSprite("Eagle_Elive"),
						   Resources::ResourceManager::getSprite("Eagle_Dead") }
	{ }

	void Eagle::render() const
	{
		m_eagle_sprites[static_cast<size_t>(m_eagle_state)]->render(m_position, m_size, m_rotation);
	}

	void Eagle::update(const uint64_t delta)
	{ }
}