#include "BrickWall.h"

#include "../../Render/Sprite2D.h"

#include "../../Resources/ResourceManager.h"

namespace BatleCity
{
	BrickWall::BrickWall(const EBrickWallType brick_type, const glm::vec2& position,
						 const glm::vec2& size, const float rotation) : 
																		IGameObject(position, size, rotation),
																		m_brick_type(brick_type)
	{ 
		m_sprite = Resources::ResourceManager::getSprite("BrickWall_8x8");
	}



	void BrickWall::render() const
	{
		switch (m_brick_type)
		{
		case BatleCity::BrickWall::EBrickWallType::All:
			m_sprite->render(m_position, glm::vec2(m_size.x / 2, m_size.y / 2), m_rotation);
			m_sprite->render(m_position + glm::vec2(m_size.x / 2,0.f), glm::vec2(m_size.x / 2, m_size.y / 2), m_rotation);
			m_sprite->render(m_position + glm::vec2(0.f, m_size.y / 2), glm::vec2(m_size.x / 2, m_size.y / 2), m_rotation);
			m_sprite->render(m_position + glm::vec2(m_size.x / 2, m_size.y / 2), glm::vec2(m_size.x / 2, m_size.y / 2), m_rotation);
			break;
		case BatleCity::BrickWall::EBrickWallType::Top:
			m_sprite->render(m_position + glm::vec2(0.f, m_size.y / 2), glm::vec2(m_size.x / 2, m_size.y / 2), m_rotation);
			m_sprite->render(m_position + glm::vec2(m_size.x / 2, m_size.y / 2), glm::vec2(m_size.x / 2, m_size.y / 2), m_rotation);
			break;
		case BatleCity::BrickWall::EBrickWallType::Right:
			m_sprite->render(m_position + glm::vec2(m_size.x / 2, 0.f), glm::vec2(m_size.x / 2, m_size.y / 2), m_rotation);
			m_sprite->render(m_position + glm::vec2(m_size.x / 2, m_size.y / 2), glm::vec2(m_size.x / 2, m_size.y / 2), m_rotation);
			break;
		case BatleCity::BrickWall::EBrickWallType::Bottom:
			m_sprite->render(m_position, glm::vec2(m_size.x / 2, m_size.y / 2), m_rotation);
			m_sprite->render(m_position + glm::vec2(m_size.x / 2, 0.f), glm::vec2(m_size.x / 2, m_size.y / 2), m_rotation);
			break;
		case BatleCity::BrickWall::EBrickWallType::Left:
			m_sprite->render(m_position, glm::vec2(m_size.x / 2, m_size.y / 2), m_rotation);
			m_sprite->render(m_position + glm::vec2(0.f, m_size.y / 2), glm::vec2(m_size.x / 2, m_size.y / 2), m_rotation);
			break;
		case BatleCity::BrickWall::EBrickWallType::LeftTop:
			break;
		case BatleCity::BrickWall::EBrickWallType::LeftBottom:
			break;
		case BatleCity::BrickWall::EBrickWallType::RightBottom:
			break;
		case BatleCity::BrickWall::EBrickWallType::RightTop:
			break;
		case BatleCity::BrickWall::EBrickWallType::LeftTop_RightBottom:
			break;
		case BatleCity::BrickWall::EBrickWallType::RightTop_LeftBottom:
			break;
		case BatleCity::BrickWall::EBrickWallType::WithoutLeftTop:
			break;
		case BatleCity::BrickWall::EBrickWallType::WithoutLeftBottom:
			break;
		case BatleCity::BrickWall::EBrickWallType::WithoutRightBottom:
			break;
		case BatleCity::BrickWall::EBrickWallType::WithoutRightTop:
			break;
		case BatleCity::BrickWall::EBrickWallType::Nothing:
			break;
		default:
			break;
		}
	}



	void BrickWall::update(const uint64_t delta)
	{ }
}