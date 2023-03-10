#pragma once 
#ifndef BETON_WALL_H
#define BETON_WALL_H

#include <memory>

#include "Wall.h"

#include "../../Render/Sprite2D.h"

namespace BatleCity
{
	class BetonWall : public Wall
	{
	public:

		BetonWall(const EWallType wall_type, const glm::vec2& position, const glm::vec2& size, const float rotation);

		void renderBlock(const EBlockLocation block_location) const override;

	private:

		std::shared_ptr<RenderEngine::Sprite2D> m_sprite;

	};
}

#endif // !BRICK_WALL_H
