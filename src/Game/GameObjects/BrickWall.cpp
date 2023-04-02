#include "BrickWall.h"

#include "../../Resources/ResourceManager.h"

namespace BatleCity
{
	BrickWall::BrickWall(const EBlockType wall_type, const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer)
		: Block(EGameObjectType::BrickWall, wall_type, position, size, rotation, layer)
		, m_sprite(Resources::ResourceManager::getSprite("BrickWall_8x8"))
	{ }

	void BrickWall::renderBlock(const ESubBlockLocation subBlock_location) const
	{
		switch (subBlock_location)
		{
		case ESubBlockLocation::LeftTop:
			m_sprite->render(m_position + glm::vec2(0.f, m_size.y / 2), m_size / 2.f, m_rotation, m_layer);
			break;
		case ESubBlockLocation::RightBottom:
			m_sprite->render(m_position + glm::vec2(m_size.x / 2, 0.f), m_size / 2.f, m_rotation, m_layer);
			break;
		case ESubBlockLocation::RightTop:
			m_sprite->render(m_position + m_size / 2.f, m_size / 2.f, m_rotation, m_layer);
			break;
		case ESubBlockLocation::LeftBottom:
			m_sprite->render(m_position, m_size / 2.f, m_rotation, m_layer);
			break;
		default:
			break;
		}
	}



	bool BrickWall::onCollision(EGameObjectType game_object_type, const glm::vec2& direction)
	{
		if (game_object_type == EGameObjectType::Bullet)
		{
			changeBrickWallAfterCollisionWithBullet(direction);
		}
		return true;
	}



	void BrickWall::changeBrickWallAfterCollisionWithBullet(const glm::vec2& direction)
	{
		switch (m_block_type)
		{
		case BatleCity::Block::EBlockType::All:
			if (direction == glm::vec2(-1.f, 0.f))
			{
				m_colliders.swap(std::vector{ Physics::AABB(glm::vec2(0.f), glm::vec2(m_size.x / 2.f, m_size.y)) });
				m_block_type = EBlockType::Left;
			}
			else if (direction == glm::vec2(1.f, 0.f))
			{
				m_colliders.swap(std::vector{ Physics::AABB(glm::vec2(m_size.x / 2.f, 0.f), m_size) });
				m_block_type = EBlockType::Right;
			}
			else if (direction == glm::vec2(0.f, 1.f))
			{
				m_colliders.swap(std::vector{ Physics::AABB(glm::vec2(0.f, m_size.y / 2.f), m_size) });
				m_block_type = EBlockType::Top;
			}
			else if (direction == glm::vec2(0.f, -1.f))
			{
				m_colliders.swap(std::vector{ Physics::AABB(glm::vec2(0.f), glm::vec2(m_size.x, m_size.y / 2.f)) });
				m_block_type = EBlockType::Bottom;
			}
			break;
		case BatleCity::Block::EBlockType::Top:
			if (direction == glm::vec2(0.f, 1.f) || direction == glm::vec2(0.f, -1.f))
			{
				m_colliders.deleteAllColliders();
				m_block_type = EBlockType::Nothing;
			}
			else if (direction == glm::vec2(1.f, 0.f))
			{
				m_colliders.swap(std::vector{ Physics::AABB(m_size / 2.f, m_size) });
				m_block_type = EBlockType::RightTop;
			}
			else if (direction == glm::vec2(-1.f, 0.f))
			{
				m_colliders.swap(std::vector{ Physics::AABB(glm::vec2(0.f, m_size.y / 2.f), glm::vec2(m_size.x / 2, m_size.y)) });
				m_block_type = EBlockType::LeftTop;
			}
			break;
		case BatleCity::Block::EBlockType::Right:
			if (direction == glm::vec2(-1.f, 0.f) || direction == glm::vec2(1.f, 0.f))
			{
				m_colliders.deleteAllColliders();
				m_block_type = EBlockType::Nothing;
			}
			else if (direction == glm::vec2(0.f, -1.f))
			{
				m_colliders.swap(std::vector{ Physics::AABB(glm::vec2(m_size.x / 2.f, 0.f), glm::vec2(m_size.x, m_size.y / 2.f)) });
				m_block_type = EBlockType::RightBottom;
			}
			else if (direction == glm::vec2(0.f, 1.f))
			{
				m_colliders.swap(std::vector{ Physics::AABB(m_size / 2.f, m_size) });
				m_block_type = EBlockType::RightTop;
			}
			break;
		case BatleCity::Block::EBlockType::Bottom:
			if (direction == glm::vec2(0.f, 1.f) || direction == glm::vec2(0.f, -1.f))
			{
				m_colliders.deleteAllColliders();
				m_block_type = EBlockType::Nothing;
			}
			else if (direction == glm::vec2(1.f, 0.f))
			{
				m_colliders.swap(std::vector{ Physics::AABB(glm::vec2(m_size.x / 2.f, 0.f), glm::vec2(m_size.x, m_size.y / 2.f)) });
				m_block_type = EBlockType::RightBottom;
			}
			else if (direction == glm::vec2(-1.f, 0.f))
			{
				m_colliders.swap(std::vector{ Physics::AABB(glm::vec2(0.f), m_size / 2.f) });
				m_block_type = EBlockType::LeftBottom;
			}
			break;
		case BatleCity::Block::EBlockType::Left:
			if (direction == glm::vec2(-1.f, 0.f) || direction == glm::vec2(1.f, 0.f))
			{
				m_colliders.deleteAllColliders();
				m_block_type = EBlockType::Nothing;
			}
			else if (direction == glm::vec2(0.f, -1.f))
			{
				m_colliders.swap(std::vector{ Physics::AABB(glm::vec2(0.f), m_size / 2.f) });
				m_block_type = EBlockType::LeftBottom;
			}
			else if (direction == glm::vec2(0.f, 1.f))
			{
				m_colliders.swap(std::vector{ Physics::AABB(glm::vec2(0.f, m_size.y / 2.f), glm::vec2(m_size.x / 2.f, m_size.y)) });
				m_block_type = EBlockType::LeftTop;
			}
			break;
		default:
			m_colliders.deleteAllColliders();
			m_block_type = EBlockType::Nothing;
			break;
		}
	}
}