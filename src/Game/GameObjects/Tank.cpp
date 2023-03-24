#include "Tank.h"

#include "Bullet.h"

#include "../../Render/Sprite2D.h"

#include "../../Resources/ResourceManager.h"

#include "../../Physics/PhysicsEngine.h"

namespace BatleCity
{
	Tank::Tank(std::shared_ptr<RenderEngine::Sprite2D> sprite_ptr, const glm::vec2& positiion, const glm::vec2& size,
		       const double max_velocity, double delay_between_shots, const glm::vec2& direction, double velocity, const float layer)

		  : IDynamicGameObject(EGameObjectType::Tank, positiion, size, 0.f, layer, direction, velocity)
		  ,	m_tank_sprite(std::move(sprite_ptr))
		  ,	m_respawn_animation{ RenderEngine::SpriteAnimator(Resources::ResourceManager::getSprite("RespawnAnimation")), my_system::Timer()}
		  ,	m_shield_animation{ RenderEngine::SpriteAnimator(Resources::ResourceManager::getSprite("ShieldAnimation")), my_system::Timer() }
		  ,	m_max_velocity(max_velocity)
		  , m_delay_between_shots(delay_between_shots)
		  ,	m_current_orientation(EOrientation::Top)
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

		m_timer_for_shots.setCallBack([&]() {
				m_is_fair = false;
			});

		m_colliders.addCollider(glm::vec2(0.f), m_size);
	}


	
	void Tank::setOrientation(const Tank::EOrientation orietation)
	{
		if (!m_is_respawn)
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
				m_direction.x = 0.f;		m_direction.y = 1.f;
				break;
			case Tank::EOrientation::Right:
				m_tank_sprite.setState("tankRightState");
				m_direction.x = 1.f;		m_direction.y = 0.f;
				break;
			case Tank::EOrientation::Bottom:
				m_tank_sprite.setState("tankBottomState");
				m_direction.x = 0.f;		m_direction.y = -1.f;
				break;
			case Tank::EOrientation::Left:
				m_tank_sprite.setState("tankLeftState");
				m_direction.x = -1.f;		m_direction.y = 0.f;
				break;
			default:
				break;
			}
		}
	}



	void Tank::setVelocity(double velocity)
	{
		if (!m_is_respawn)		m_velocity = velocity;
	}



	double Tank::getMaxVelocity() const
	{
		return m_max_velocity;
	}



	void Tank::update(const double delta)
	{
		if (m_is_fair)
		{
			m_timer_for_shots.update(delta);
		}
		if (m_is_respawn)
		{
			m_respawn_animation.first.update(delta);
			m_respawn_animation.second.update(delta);
		}
		else
		{
			if (m_velocity > 0)
			{
				m_tank_sprite.update(delta);
			}
			if (m_has_shild)
			{
				m_shield_animation.first.update(delta);
				m_shield_animation.second.update(delta);
			}
			m_bullets.updateBullets(delta);
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

			m_bullets.renderBullets();
		}
	}



	void Tank::fair() const
	{
		if (!m_is_respawn && !m_is_fair)
		{
			m_is_fair = true;
			auto bullet = std::make_shared<Bullet>(m_current_orientation, m_size / 2.f, m_layer, 3 * m_max_velocity);
			bullet->fire(m_position, m_direction, bullet->getMaxVelocity());
			m_bullets.addBullet(bullet);
			Physics::PhysicsEngine::addDynamicGameObject(std::move(bullet));

			m_timer_for_shots.start(m_delay_between_shots);
		}
	}



	void Tank::onCollision(EGameObjectType game_object_type)
	{

	}
}