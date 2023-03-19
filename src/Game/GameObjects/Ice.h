#pragma once
#ifndef ICE_H
#define ICE_H

#include <memory>

#include "Block.h"

#include "../../Render/Sprite2D.h"

namespace BatleCity
{
	class Ice : public Block
	{
	public:

		Ice(const EBlockType ice_type, const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer);

	private:

		void renderBlock(const ESubBlockLocation subBlock_location) const override;

		std::shared_ptr<RenderEngine::Sprite2D> m_sprite;

	};
}

#endif // !ICE_H