#pragma once
#ifndef WATER_H
#define WATER_H

#include <memory>

#include "Block.h"

#include "../../Render/SpriteAnimator.h"

namespace BatleCity
{
	class Water : public Block
	{
	public:

		Water(const EBlockType ice_type, const glm::vec2& position, const glm::vec2& size, const float rotation);

		void update(const uint64_t delta) override;

	private:

		void renderBlock(const ESubBlockLocation subBlock_location) const override;



		RenderEngine::SpriteAnimator m_sprite;

	};
}

#endif // !WATER_H