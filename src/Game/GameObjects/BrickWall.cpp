#include "BrickWall.h"

#include "../../Render/Sprite2D.h"

namespace BatleCity
{
	BrickWall::BrickWall(std::shared_ptr<RenderEngine::Sprite2D> sprite_ptr, const glm::vec2& position,
						 const glm::vec2& size, const float rotation) : 
																		IGameObject(position, size, rotation),
																		m_current_sprite(std::move(sprite_ptr))
	{ }



	void BrickWall::render() const
	{
		m_current_sprite->render(m_position, m_size, m_rotation);
	}



	void BrickWall::update(const uint64_t delta)
	{ }
}