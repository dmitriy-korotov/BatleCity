#include "Tank.h"

#include "../../Render/Sprite2D.h"

#include "../../Resources/ResourceManager.h"

namespace BatleCity
{
	Tank::Tank(std::shared_ptr<RenderEngine::Sprite2D> sprite_ptr, const glm::vec2& positiion, const glm::vec2& size, const double velocity, const float layer)
	  : IGameObject(positiion, size, 0.f, layer),
		m_tank_sprite(std::move(sprite_ptr)),
		m_respawn_animation{ RenderEngine::SpriteAnimator(Resources::ResourceManager::getSprite("RespawnAnimation")), my_system::Timer()},
		m_shield_animation{ RenderEngine::SpriteAnimator(Resources::ResourceManager::getSprite("ShieldAnimation")), my_system::Timer() },
		m_velocity(velocity),
		m_current_orientation(EOrientation::Top),
		m_move(false),
		m_is_respawn(true),
		m_has_shild(false),
		m_move_offset(glm::vec2(0.f, 1.f))
	{
		setPosition(m_position);

		m_respawn_animation.first.setState("default");
		m_respawn_animation.second.setCallBack([&]() {
			m_is_respawn = false;
			m_has_shild = true;
			m_shield_animation.second.start(3000);
		});
		m_respawn_animation.second.start(1500);

		m_shield_animation.first.setState("default");
		m_shield_animation.second.setCallBack([&]() {
			m_has_shild = false;
		});
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
			m_tank_sprite.setState("tankTopState");
			m_move_offset.x = 0.f;		m_move_offset.y = 1.f;
			break;
		case Tank::EOrientation::Right:
			m_tank_sprite.setState("tankRightState");
			m_move_offset.x = 1.f;		m_move_offset.y = 0.f;
			break;
		case Tank::EOrientation::Bottom:
			m_tank_sprite.setState("tankBottomState");
			m_move_offset.x = 0.f;		m_move_offset.y = -1.f;
			break;
		case Tank::EOrientation::Left:
			m_tank_sprite.setState("tankLeftState");
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



	void Tank::update(const double delta)
	{
		if (m_is_respawn)
		{
			m_respawn_animation.first.update(delta);
			m_respawn_animation.second.update(delta);
		}
		else
		{
			if (m_move)
			{
				m_position += static_cast<float>(delta * m_velocity) * m_move_offset;
				m_tank_sprite.update(delta);
			}
			if (m_has_shild)
			{
				m_shield_animation.first.update(delta);
				m_shield_animation.second.update(delta);
			}
		}
	}



	void Tank::render() const
	{
		if (m_is_respawn)
		{
			m_respawn_animation.first.render(m_position, m_size, m_rotation, m_layer);
		}
		else
		{
			if (m_has_shild)
			{
				m_shield_animation.first.render(m_position, m_size, m_rotation, m_layer);
			}
			m_tank_sprite.render(m_position, m_size, m_rotation, m_layer);
		}
	}
}