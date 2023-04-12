#pragma once 
#ifndef BRICK_WALL_H
#define BRICK_WALL_H

#include <memory>
#include <vector>

#include "Block.h"

#include "../../Render/Sprite2D.h"

namespace BatleCity
{
	class BrickWall : public Block
	{
	public:

		BrickWall(const EBlockType wall_type, const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer);

		void renderBlock(const ESubBlockLocation subBlock_location) const override;
		bool onCollision(EGameObjectType game_object_type, std::shared_ptr<Physics::AABB> target_collider, const glm::vec2& direction) override;

	private:

		glm::vec2 getOffsetBySubBlockLocation(ESubBlockLocation subBlockLocation) const noexcept;
		ESubBlockLocation getSubBlockLocationByCollider(std::shared_ptr<Physics::AABB>& collider) const noexcept;
		size_t getColliderIndexBySubBlockLocation(ESubBlockLocation subBlock_location) const noexcept;
		void changeBrickWallAfterCollisionWithBullet(ESubBlockLocation& subBlock_location, const glm::vec2& direction);

		std::shared_ptr<RenderEngine::Sprite2D> m_left_bottom_sprite	= nullptr;
		std::shared_ptr<RenderEngine::Sprite2D> m_left_top_sprite		= nullptr;
		std::shared_ptr<RenderEngine::Sprite2D> m_right_bottom_sprite	= nullptr;
		std::shared_ptr<RenderEngine::Sprite2D> m_right_top_sprite		= nullptr;

		std::vector<EBlockType> m_subBlock_types = { EBlockType::All, EBlockType::All, EBlockType::All, EBlockType::All };

	};
}

#endif // !BRICK_WALL_H
