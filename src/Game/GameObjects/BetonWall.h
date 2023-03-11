#pragma once 
#ifndef BETON_WALL_H
#define BETON_WALL_H

#include <memory>

#include "Block.h"

#include "../../Render/Sprite2D.h"

namespace BatleCity
{
	class BetonWall : public Block
	{
	public:

		BetonWall(const EBlockType wall_type, const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer);

		void renderBlock(const ESubBlockLocation subBlock_location) const override;

	private:

		std::shared_ptr<RenderEngine::Sprite2D> m_sprite;

	};
}

#endif // !BRICK_WALL_H
