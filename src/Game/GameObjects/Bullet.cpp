#include "Bullet.h"

#include "../../Resources/ResourceManager.h"

namespace BatleCity
{
	Bullet::Bullet(EOrientation bullet_type, const glm::vec2& size, float layer, double max_velocity)
		: IDynamicGameObject(glm::vec2(0.f), size, 0.f, layer, glm::vec2(0.f, 1.f), 0)
		, m_max_velocity(max_velocity)
	{ 
		switch (bullet_type)
		{
		case BatleCity::Bullet::EOrientation::Top:
			m_sprite = Resources::ResourceManager::getSprite("Bullet_Top_8x8");
			break;
		case BatleCity::Bullet::EOrientation::Bottom:
			m_sprite = Resources::ResourceManager::getSprite("Bullet_Bottom_8x8");
			break;
		case BatleCity::Bullet::EOrientation::Left:
			m_sprite = Resources::ResourceManager::getSprite("Bullet_Left_8x8");
			break;
		case BatleCity::Bullet::EOrientation::Right:
			m_sprite = Resources::ResourceManager::getSprite("Bullet_Right_8x8");
			break;
		default:
			break;
		}

		m_colliders.emplace_back(glm::vec2(0.f), m_size);
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
	{ }



	void Bullet::render() const
	{
		if (m_is_fire)
		{
			m_sprite->render(m_position + m_size / 2.f + m_direction * m_size.x, m_size, m_rotation, m_layer);
		}
	}



	void Bullet::onCollision()
	{
		m_sprite.reset();
		m_is_fire = false;
	}
}