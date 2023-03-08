#include "Tank.h"

#include "../../Render/AnimatedSprite2D.h"

namespace BatleCity
{
	Tank::Tank(std::shared_ptr<RenderEngine::AnimatedSprite2D> sprite, const glm::vec2& positiion, const glm::vec2& size, const float velocity)
	  : IGameObject(positiion, size, 0.f),
		m_sprite(std::move(sprite)),
		m_velocity(velocity),
		m_current_orientation(EOrientation::Top),
		m_move(false),
		m_move_offset(glm::vec2(0.f, 1.f))
	{
		setPosition(m_position);
	}


	
	void Tank::setOrientation(const Tank::EOrientation orietation)
	{
		if (m_current_orientation == orietation)
		{
			return;
		}

		m_current_orientation = orietation;

		switch (m_current_orientation)
		{
		case Tank::EOrientation::Top:
			m_sprite->setState("tankTopState");
			m_move_offset.x = 0.f;		m_move_offset.y = 1.f;
			break;
		case Tank::EOrientation::Right:
			m_sprite->setState("tankRightState");
			m_move_offset.x = 1.f;		m_move_offset.y = 0.f;
			break;
		case Tank::EOrientation::Bottom:
			m_sprite->setState("tankBottomState");
			m_move_offset.x = 0.f;		m_move_offset.y = -1.f;
			break;
		case Tank::EOrientation::Left:
			m_sprite->setState("tankLeftState");
			m_move_offset.x = -1.f;		m_move_offset.y = 0.f;
			break;
		default:
			break;
		}
	}



	void Tank::move(const bool move)
	{
		m_move = move;
	}



	void Tank::update(const uint64_t delta)
	{
		if (m_move)
		{
			m_position += delta * m_velocity * m_move_offset;
			m_sprite->update(delta);
		}
	}



	void Tank::render() const
	{
		m_sprite->render(m_position, m_size, m_rotation);
	}
}