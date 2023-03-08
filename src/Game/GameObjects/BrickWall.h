#pragma once
#ifndef BRICK_WALL_H
#define BRICK_WALL_H

#include <memory>

#include "IGameObject.h"

namespace BatleCity
{
	class BrickWall : public IGameObject
	{
	public:

		BrickWall(std::shared_ptr<RenderEngine::Sprite2D> sprite_ptr, const glm::vec2& position, const glm::vec2& size, const float rotation);

		void update(const uint64_t delta) override;
		void render() const override;

	private:
	
		std::shared_ptr<RenderEngine::Sprite2D> m_current_sprite;
		
	};
}

#endif // BRICK_WALL_H