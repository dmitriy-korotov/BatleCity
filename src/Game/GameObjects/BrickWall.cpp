#include "BrickWall.h"

#include "../../Resources/ResourceManager.h"

namespace BatleCity
{
	BrickWall::BrickWall(const EBlockType wall_type, const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer)
		: Block(EGameObjectType::BrickWall, wall_type, position, size, rotation, layer)
		, m_left_bottom_sprite(Resources::ResourceManager::getSprite("BrickWall_Left_Bottom_8x8"))
		, m_left_top_sprite(Resources::ResourceManager::getSprite("BrickWall_Left_Top_8x8"))
		, m_right_bottom_sprite(Resources::ResourceManager::getSprite("BrickWall_Right_Bottom_8x8"))
		, m_right_top_sprite(Resources::ResourceManager::getSprite("BrickWall_Right_Top_8x8"))
	{ 
		m_colliders.deleteAllColliders();

		switch (wall_type)
		{
		case BatleCity::Block::EBlockType::All:
			m_colliders.addCollider(glm::vec2(0.f), m_size / 2.f);
			m_colliders.addCollider(glm::vec2(0.f, m_size.y / 2.f), glm::vec2(m_size.x / 2.f, m_size.y));
			m_colliders.addCollider(m_size / 2.f, m_size);
			m_colliders.addCollider(glm::vec2(m_size.x / 2.f, 0.f), glm::vec2(m_size.x, m_size.y / 2.f));
			break;
		case BatleCity::Block::EBlockType::Top:
			m_colliders.addCollider(glm::vec2(0.f, m_size.y / 2.f), glm::vec2(m_size.x / 2.f, m_size.y));
			m_colliders.addCollider(m_size / 2.f, m_size);
			break;
		case BatleCity::Block::EBlockType::Right:
			m_colliders.addCollider(m_size / 2.f, m_size);
			m_colliders.addCollider(glm::vec2(m_size.x / 2.f, 0.f), glm::vec2(m_size.x, m_size.y / 2.f));
			break;
		case BatleCity::Block::EBlockType::Bottom:
			m_colliders.addCollider(glm::vec2(0.f), m_size / 2.f);
			m_colliders.addCollider(glm::vec2(m_size.x / 2.f, 0.f), glm::vec2(m_size.x, m_size.y / 2.f));
			break;
		case BatleCity::Block::EBlockType::Left:
			m_colliders.addCollider(glm::vec2(0.f), m_size / 2.f);
			m_colliders.addCollider(glm::vec2(0.f, m_size.y / 2.f), glm::vec2(m_size.x / 2.f, m_size.y));
			break;
		case BatleCity::Block::EBlockType::LeftTop:
			m_colliders.addCollider(glm::vec2(0.f, m_size.y / 2.f), glm::vec2(m_size.x / 2.f, m_size.y));
			break;
		case BatleCity::Block::EBlockType::LeftBottom:
			m_colliders.addCollider(glm::vec2(0.f), m_size / 2.f);
			break;
		case BatleCity::Block::EBlockType::RightBottom:
			m_colliders.addCollider(glm::vec2(m_size.x / 2.f, 0.f), glm::vec2(m_size.x, m_size.y / 2.f));
			break;
		case BatleCity::Block::EBlockType::RightTop:
			m_colliders.addCollider(m_size / 2.f, m_size);
			break;
		case BatleCity::Block::EBlockType::LeftTop_RightBottom:
			m_colliders.addCollider(glm::vec2(0.f, m_size.y / 2.f), glm::vec2(m_size.x / 2.f, m_size.y));
			m_colliders.addCollider(glm::vec2(m_size.x / 2.f, 0.f), glm::vec2(m_size.x, m_size.y / 2.f));
			break;
		case BatleCity::Block::EBlockType::RightTop_LeftBottom:
			m_colliders.addCollider(glm::vec2(0.f), m_size / 2.f);
			m_colliders.addCollider(m_size / 2.f, m_size);
			break;
		case BatleCity::Block::EBlockType::WithoutLeftTop:
			m_colliders.addCollider(glm::vec2(0.f), m_size / 2.f);
			m_colliders.addCollider(m_size / 2.f, m_size);
			m_colliders.addCollider(glm::vec2(m_size.x / 2.f, 0.f), glm::vec2(m_size.x, m_size.y / 2.f));
			break;
		case BatleCity::Block::EBlockType::WithoutLeftBottom:
			m_colliders.addCollider(glm::vec2(0.f, m_size.y / 2.f), glm::vec2(m_size.x / 2.f, m_size.y));
			m_colliders.addCollider(m_size / 2.f, m_size);
			m_colliders.addCollider(glm::vec2(m_size.x / 2.f, 0.f), glm::vec2(m_size.x, m_size.y / 2.f));
			break;
		case BatleCity::Block::EBlockType::WithoutRightBottom:
			m_colliders.addCollider(glm::vec2(0.f), m_size / 2.f);
			m_colliders.addCollider(glm::vec2(0.f, m_size.y / 2.f), glm::vec2(m_size.x / 2.f, m_size.y));
			m_colliders.addCollider(m_size / 2.f, m_size);
			break;
		case BatleCity::Block::EBlockType::WithoutRightTop:
			m_colliders.addCollider(glm::vec2(0.f), m_size / 2.f);
			m_colliders.addCollider(glm::vec2(0.f, m_size.y / 2.f), glm::vec2(m_size.x / 2.f, m_size.y));
			m_colliders.addCollider(glm::vec2(m_size.x / 2.f, 0.f), glm::vec2(m_size.x, m_size.y / 2.f));
			break;
		case BatleCity::Block::EBlockType::Nothing:
			break;
		}
	}

	void BrickWall::renderBlock(const ESubBlockLocation subBlock_location) const
	{
		glm::vec2 offset = getOffsetBySubBlockLocation(subBlock_location);

		switch (m_subBlock_types[static_cast<size_t>(subBlock_location)])
		{
		case EBlockType::All:
			m_left_bottom_sprite->render(m_position + offset, m_size / 2.f, m_rotation, m_layer);
			m_left_top_sprite->render(m_position + offset, m_size / 2.f, m_rotation, m_layer);
			m_right_bottom_sprite->render(m_position + offset, m_size / 2.f, m_rotation, m_layer);
			m_right_top_sprite->render(m_position + offset, m_size / 2.f, m_rotation, m_layer);
			break;
		case EBlockType::Left:
			m_left_bottom_sprite->render(m_position + offset, m_size / 2.f, m_rotation, m_layer);
			m_left_top_sprite->render(m_position + offset, m_size / 2.f, m_rotation, m_layer);
			break;
		case EBlockType::Bottom:
			m_left_bottom_sprite->render(m_position + offset, m_size / 2.f, m_rotation, m_layer);
			m_right_bottom_sprite->render(m_position + offset, m_size / 2.f, m_rotation, m_layer);
			break;
		case EBlockType::Right:
			m_right_bottom_sprite->render(m_position + offset, m_size / 2.f, m_rotation, m_layer);
			m_right_top_sprite->render(m_position + offset, m_size / 2.f, m_rotation, m_layer);
			break;
		case EBlockType::Top:
			m_left_top_sprite->render(m_position + offset, m_size / 2.f, m_rotation, m_layer);
			m_right_top_sprite->render(m_position + offset, m_size / 2.f, m_rotation, m_layer);
			break;
		case EBlockType::LeftBottom:
			m_left_bottom_sprite->render(m_position + offset, m_size / 2.f, m_rotation, m_layer);
			break;
		case EBlockType::RightBottom:
			m_right_bottom_sprite->render(m_position + offset, m_size / 2.f, m_rotation, m_layer);
			break;
		case EBlockType::RightTop:
			m_right_top_sprite->render(m_position + offset, m_size / 2.f, m_rotation, m_layer);
			break;
		case EBlockType::LeftTop:
			m_left_top_sprite->render(m_position + offset, m_size / 2.f, m_rotation, m_layer);
			break;
		}
	}



	bool BrickWall::onCollision(EGameObjectType game_object_type, std::shared_ptr<Physics::AABB> target_collider, const glm::vec2& direction)
	{
		if (game_object_type == EGameObjectType::Bullet)
		{
			ESubBlockLocation location = getSubBlockLocationByCollider(target_collider);
			changeBrickWallAfterCollisionWithBullet(location, direction);
		}
		return true;
	}



	glm::vec2 BrickWall::getOffsetBySubBlockLocation(ESubBlockLocation subBlock_location) const noexcept
	{
		glm::vec2 offset(0.f);
		switch (subBlock_location)
		{
		case ESubBlockLocation::LeftTop:
			offset = glm::vec2(0.f, m_size.y / 2.f);
			break;
		case ESubBlockLocation::RightBottom:
			offset = glm::vec2(m_size.x / 2, 0.f);
			break;
		case ESubBlockLocation::RightTop:
			offset = m_size / 2.f;
			break;
		case ESubBlockLocation::LeftBottom:
			break;
		}
		return offset;
	}



	BrickWall::ESubBlockLocation BrickWall::getSubBlockLocationByCollider(std::shared_ptr<Physics::AABB>& collider) const noexcept
	{
		if (collider->getLeftBottom().x >= 0.f && collider->getLeftBottom().x < m_size.x / 2.f
				&&
			collider->getRightTop().y > 0.f && collider->getRightTop().y <= m_size.y / 2.f)
		{
			return ESubBlockLocation::LeftBottom;
		}
		else if (collider->getLeftBottom().x >= m_size.x / 2.f && collider->getLeftBottom().x < m_size.x
					&&
				 collider->getRightTop().y > m_size.y / 2.f && collider->getRightTop().y <= m_size.y)
		{
			return ESubBlockLocation::RightTop;
		}
		else if (collider->getLeftBottom().x >= m_size.x / 2.f && collider->getLeftBottom().x < m_size.x
					&&
				 collider->getRightTop().y > 0 && collider->getRightTop().y <= m_size.y / 2.f)
		{
			return ESubBlockLocation::RightBottom;
		}
		else if (collider->getLeftBottom().x >= 0.f && collider->getLeftBottom().x < m_size.x / 2.f
					&&
				 collider->getRightTop().y > m_size.y / 2.f && collider->getRightTop().y <= m_size.y)
		{
			return ESubBlockLocation::LeftTop;
		}
		return ESubBlockLocation::LeftBottom;
	}



	size_t BrickWall::getColliderIndexBySubBlockLocation(ESubBlockLocation subBlock_location) const noexcept
	{
		switch (m_block_type)
		{
		case BatleCity::Block::EBlockType::All:
			return static_cast<size_t>(subBlock_location);
		case BatleCity::Block::EBlockType::Top:
			return static_cast<size_t>(subBlock_location) - 1;
		case BatleCity::Block::EBlockType::Right:
			return static_cast<size_t>(subBlock_location) - 2;
		case BatleCity::Block::EBlockType::Bottom:
			switch (subBlock_location)
			{
			case BatleCity::Block::ESubBlockLocation::RightBottom:
				return 1;
			case BatleCity::Block::ESubBlockLocation::LeftBottom:
				return 0;
			}
			break;
		case BatleCity::Block::EBlockType::Left:
			return static_cast<size_t>(subBlock_location);
		case BatleCity::Block::EBlockType::LeftTop:
			return 0;
		case BatleCity::Block::EBlockType::LeftBottom:
			return 0;
		case BatleCity::Block::EBlockType::RightBottom:
			return 0;
		case BatleCity::Block::EBlockType::RightTop:
			return 0;
		case BatleCity::Block::EBlockType::LeftTop_RightBottom:
			switch (subBlock_location)
			{
			case BatleCity::Block::ESubBlockLocation::LeftTop:
				return 0;
			case BatleCity::Block::ESubBlockLocation::RightBottom:
				return 1;
			}
		case BatleCity::Block::EBlockType::RightTop_LeftBottom:
			switch (subBlock_location)
			{
			case BatleCity::Block::ESubBlockLocation::RightTop:
				return 1;
			case BatleCity::Block::ESubBlockLocation::LeftBottom:
				return 0;
			}
		case BatleCity::Block::EBlockType::WithoutLeftTop:
			switch (subBlock_location)
			{
			case BatleCity::Block::ESubBlockLocation::RightTop:
				return 1;
			case BatleCity::Block::ESubBlockLocation::RightBottom:
				return 2;
			case BatleCity::Block::ESubBlockLocation::LeftBottom:
				return 0;
			}
		case BatleCity::Block::EBlockType::WithoutLeftBottom:
			switch (subBlock_location)
			{
			case BatleCity::Block::ESubBlockLocation::RightTop:
				return 1;
			case BatleCity::Block::ESubBlockLocation::RightBottom:
				return 2;
			case BatleCity::Block::ESubBlockLocation::LeftTop:
				return 0;
			}
		case BatleCity::Block::EBlockType::WithoutRightBottom:
			switch (subBlock_location)
			{
			case BatleCity::Block::ESubBlockLocation::RightTop:
				return 2;
			case BatleCity::Block::ESubBlockLocation::LeftTop:
				return 1;
			case BatleCity::Block::ESubBlockLocation::LeftBottom:
				return 0;
			}
		case BatleCity::Block::EBlockType::WithoutRightTop:
			switch (subBlock_location)
			{
			case BatleCity::Block::ESubBlockLocation::LeftTop:
				return 1;
			case BatleCity::Block::ESubBlockLocation::RightBottom:
				return 2;
			case BatleCity::Block::ESubBlockLocation::LeftBottom:
				return 0;
			}
		}
		return 0;
	}



	void BrickWall::changeBrickWallAfterCollisionWithBullet(ESubBlockLocation& subBlock_location, const glm::vec2& direction)
	{
		size_t subBlock_index = static_cast<size_t>(subBlock_location);
		size_t collider_index = getColliderIndexBySubBlockLocation(subBlock_location);

		switch (m_subBlock_types[subBlock_index])
		{
		case BatleCity::Block::EBlockType::All:
			if (direction == glm::vec2(-1.f, 0.f))
			{
				m_colliders[collider_index].setRightTopPoint(glm::vec2(
																	   (m_colliders[collider_index].getRightTop().x + m_colliders[collider_index].getLeftBottom().x) / 2.f,
																		m_colliders[collider_index].getRightTop().y)
																	  );
				m_subBlock_types[subBlock_index] = EBlockType::Left;
			}
			else if (direction == glm::vec2(1.f, 0.f))
			{
				m_colliders[collider_index].setLeftBottomPoint(glm::vec2(
																		 (m_colliders[collider_index].getRightTop().x + m_colliders[collider_index].getLeftBottom().x) / 2.f,
																		  m_colliders[collider_index].getLeftBottom().y)
																		);
				m_subBlock_types[subBlock_index] = EBlockType::Right;
			}
			else if (direction == glm::vec2(0.f, 1.f))
			{
				m_colliders[collider_index].setLeftBottomPoint(glm::vec2(
																		  m_colliders[collider_index].getLeftBottom().x,
																		 (m_colliders[collider_index].getRightTop().y + m_colliders[collider_index].getLeftBottom().y) / 2.f)
																		);
				m_subBlock_types[subBlock_index] = EBlockType::Top;
			}
			else if (direction == glm::vec2(0.f, -1.f))
			{
				m_colliders[collider_index].setRightTopPoint(glm::vec2(
																	    m_colliders[collider_index].getRightTop().x,
																	   (m_colliders[collider_index].getRightTop().y + m_colliders[collider_index].getLeftBottom().y) / 2.f)
																	  );
				m_subBlock_types[subBlock_index] = EBlockType::Bottom;
			}
			break;
		case BatleCity::Block::EBlockType::Top:
			if (direction == glm::vec2(0.f, 1.f) || direction == glm::vec2(0.f, -1.f))
			{
				m_colliders[collider_index].disable();
				m_subBlock_types[subBlock_index] = EBlockType::Nothing;
			}
			else if (direction == glm::vec2(1.f, 0.f))
			{
				m_colliders[collider_index].setLeftBottomPoint(glm::vec2(
																		 (m_colliders[collider_index].getRightTop().x + m_colliders[collider_index].getLeftBottom().x) / 2.f,
																	      m_colliders[collider_index].getLeftBottom().y)
																		);
				m_subBlock_types[subBlock_index] = EBlockType::RightTop;
			}
			else if (direction == glm::vec2(-1.f, 0.f))
			{
				m_colliders[collider_index].setRightTopPoint(glm::vec2(
																	   (m_colliders[collider_index].getRightTop().x + m_colliders[collider_index].getLeftBottom().x) / 2.f,
																		m_colliders[collider_index].getRightTop().y)
																	  );
				m_subBlock_types[subBlock_index] = EBlockType::LeftTop;
			}
			break;
		case BatleCity::Block::EBlockType::Right:
			if (direction == glm::vec2(-1.f, 0.f) || direction == glm::vec2(1.f, 0.f))
			{
				m_colliders[collider_index].disable();
				m_subBlock_types[subBlock_index] = EBlockType::Nothing;
			}
			else if (direction == glm::vec2(0.f, -1.f))
			{
				m_colliders[collider_index].setRightTopPoint(glm::vec2(
																		m_colliders[collider_index].getRightTop().x,
																	   (m_colliders[collider_index].getRightTop().y + m_colliders[collider_index].getLeftBottom().y) / 2.f)
																	  );
				m_subBlock_types[subBlock_index] = EBlockType::RightBottom;
			}
			else if (direction == glm::vec2(0.f, 1.f))
			{
				m_colliders[collider_index].setLeftBottomPoint(glm::vec2(
																		  m_colliders[collider_index].getLeftBottom().x,
																		 (m_colliders[collider_index].getRightTop().y + m_colliders[collider_index].getLeftBottom().y) / 2.f)
																		);
				m_subBlock_types[subBlock_index] = EBlockType::RightTop;
			}
			break;
		case BatleCity::Block::EBlockType::Bottom:
			if (direction == glm::vec2(0.f, 1.f) || direction == glm::vec2(0.f, -1.f))
			{
				m_colliders[collider_index].disable();
				m_subBlock_types[subBlock_index] = EBlockType::Nothing;
			}
			else if (direction == glm::vec2(1.f, 0.f))
			{
				m_colliders[collider_index].setLeftBottomPoint(glm::vec2(
																		 (m_colliders[collider_index].getRightTop().x + m_colliders[collider_index].getLeftBottom().x) / 2.f,
																		  m_colliders[collider_index].getLeftBottom().y)
																		);
				m_subBlock_types[subBlock_index] = EBlockType::RightBottom;
			}
			else if (direction == glm::vec2(-1.f, 0.f))
			{
				m_colliders[collider_index].setRightTopPoint(glm::vec2(
																	   (m_colliders[collider_index].getRightTop().x + m_colliders[collider_index].getLeftBottom().x) / 2.f,
																		m_colliders[collider_index].getRightTop().y)
																	  );
				m_subBlock_types[subBlock_index] = EBlockType::LeftBottom;
			}
			break;
		case BatleCity::Block::EBlockType::Left:
			if (direction == glm::vec2(-1.f, 0.f) || direction == glm::vec2(1.f, 0.f))
			{
				m_colliders[collider_index].disable();
				m_subBlock_types[subBlock_index] = EBlockType::Nothing;
			}
			else if (direction == glm::vec2(0.f, -1.f))
			{
				m_colliders[collider_index].setRightTopPoint(glm::vec2(
																		m_colliders[collider_index].getRightTop().x,
																	   (m_colliders[collider_index].getRightTop().y + m_colliders[collider_index].getLeftBottom().y) / 2.f)
																	  );
				m_subBlock_types[subBlock_index] = EBlockType::LeftBottom;
			}
			else if (direction == glm::vec2(0.f, 1.f))
			{
				m_colliders[collider_index].setLeftBottomPoint(glm::vec2(
																		  m_colliders[collider_index].getLeftBottom().x,
																		 (m_colliders[collider_index].getRightTop().y + m_colliders[collider_index].getLeftBottom().y) / 2.f)
																		);
				m_subBlock_types[subBlock_index] = EBlockType::LeftTop;
			}
			break;
		default:
			m_colliders[collider_index].disable();
			m_subBlock_types[subBlock_index] = EBlockType::Nothing;
			break;
		}
	}
}