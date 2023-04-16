#include "IGameObject.h"



namespace BatleCity
{
	bool IGameObject::m_is_rendering_colliders = false;



	IGameObject::IGameObject(EGameObjectType game_object_type, const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer)
				: m_game_object_type(game_object_type)
				, m_colliders(Resources::ResourceManager::getShaderProgram("colliderShaderProgram"))
				, m_size(size)
				, m_position(position)
				, m_rotation(rotation)
				, m_layer(layer)
	{ }



	void IGameObject::renderColliders() const
	{
		if (m_is_rendering_colliders)
		{
			m_colliders.render(m_position, m_size, m_rotation, 1.f);
		}
	}
}