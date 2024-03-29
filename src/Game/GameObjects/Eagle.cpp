#include "Eagle.h"

#include "../../Resources/ResourceManager.h"

namespace BatleCity
{
	Eagle::Eagle(const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer)
		: IGameObject(EGameObjectType::Eagle, position, size, rotation, layer)
		, m_eagle_state(EEagleState::Elive)
		, m_eagle_sprites {
							Resources::ResourceManager::getSprite("Eagle_Elive"),
						    Resources::ResourceManager::getSprite("Eagle_Dead") 
						   }
	{ 
		m_colliders.addCollider(glm::vec2(0.f), m_size);
	}



	void Eagle::render() const
	{
		m_eagle_sprites[static_cast<size_t>(m_eagle_state)]->render(m_position, m_size, m_rotation, m_layer);
	}



	void Eagle::update(const double delta)
	{ }



	bool Eagle::onCollision(EGameObjectType game_object_type, std::shared_ptr<Physics::AABB> target_collider, const glm::vec2& direction)
	{
		if (game_object_type == EGameObjectType::Bullet)
		{
			m_eagle_state = EEagleState::Dead;
			m_colliders.deleteAllColliders();
		}
		return true;
	}
}