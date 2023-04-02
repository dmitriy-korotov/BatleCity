#include "Bullet.h"

#include "../../Resources/ResourceManager.h"

namespace BatleCity
{
	Bullet::Bullet(EOrientation bullet_type, const glm::vec2& size, float layer, double max_velocity)
		: IDynamicGameObject(EGameObjectType::Bullet, glm::vec2(0.f), size, 0.f, layer, glm::vec2(0.f, 1.f), 0, max_velocity)
		, m_explosion_animation{ Resources::ResourceManager::getSprite("ExplosionAnimation"), my_system::Timer() }
	{ 
		switch (bullet_type)
		{
		case BatleCity::Bullet::EOrientation::Top:
			m_sprite = Resources::ResourceManager::getSprite("Bullet_Top_8x8");
			m_colliders.addCollider(glm::vec2(m_size.x / 1.4f, m_size.y * 1.6f), glm::vec2(m_size.x * 1.2f, m_size.y * 2.2f));
			break;
		case BatleCity::Bullet::EOrientation::Bottom:
			m_sprite = Resources::ResourceManager::getSprite("Bullet_Bottom_8x8");
			m_colliders.addCollider(glm::vec2(m_size.x / 1.4f, -m_size.y / 3.5f), glm::vec2(m_size.x * 1.2f, m_size.y / 3.f));
			break;
		case BatleCity::Bullet::EOrientation::Left:
			m_sprite = Resources::ResourceManager::getSprite("Bullet_Left_8x8");
			m_colliders.addCollider(glm::vec2(-m_size.x / 4.f, m_size.y / 1.2f), glm::vec2(m_size.x  / 2.5f, m_size.y * 1.3f));
			break;
		case BatleCity::Bullet::EOrientation::Right:
			m_sprite = Resources::ResourceManager::getSprite("Bullet_Right_8x8");
			m_colliders.addCollider(glm::vec2(m_size.x * 1.7f, m_size.y / 1.2f), glm::vec2(m_size.x * 2.3f, m_size.y * 1.3f));
			break;
		default:
			break;
		}

		m_explosion_animation.first.setState("default");
		m_explosion_animation.second.setCallBack([&]() {
				m_is_destroy = true;
				m_is_exploded = false;
				m_explosion_animation.second.reset();
			});
	}



	void Bullet::fire(const glm::vec2& position, const glm::vec2& direction, double velocity)
	{
		if (!m_is_fire)
		{
			m_is_fire = true;
			m_position = position;
			m_direction = direction;
			m_velocity = velocity;
		}
	}



	void Bullet::update(double delta)
	{ 
		if (m_is_exploded)
		{
			m_explosion_animation.second.update(delta);
			m_explosion_animation.first.update(delta);
		}
	}



	void Bullet::render() const
	{
		if (m_is_fire)
		{
			m_sprite->render(m_position + m_size / 2.f + m_direction * m_size.x, m_size, m_rotation, m_layer);
		}
		if (m_is_exploded)
		{
			if (m_direction == glm::vec2(-1.f, 0.f))
			{
				m_explosion_animation.first.render(glm::vec2(m_position.x - m_size.x, m_position.y), m_size * 2.f, m_rotation, m_layer);
			}
			else if (m_direction == glm::vec2(0.f, 1.f))
			{
				m_explosion_animation.first.render(glm::vec2(m_position.x, m_position.y + m_size.y), m_size * 2.f, m_rotation, m_layer);
			}
			else if (m_direction == glm::vec2(0.f, -1.f))
			{
				m_explosion_animation.first.render(glm::vec2(m_position.x, m_position.y - m_size.y), m_size * 2.f, m_rotation, m_layer);
			}
			else
			{
				m_explosion_animation.first.render(glm::vec2(m_position.x + m_size.x, m_position.y), m_size * 2.f, m_rotation, m_layer);
			}
		}
	}



	bool Bullet::onCollision(EGameObjectType game_object_type, const glm::vec2& direction)
	{
		if (game_object_type != EGameObjectType::Water
			&& 
			game_object_type != EGameObjectType::Tree
			&&
			game_object_type != EGameObjectType::Ice)
		{
			m_velocity = 0;
			m_sprite.reset();
			m_is_fire = false;
			m_is_exploded = true;
			m_explosion_animation.second.start(m_explosion_animation.first.getDurationAnimation("default"));

			return true;
		}
		return false;
	}
}