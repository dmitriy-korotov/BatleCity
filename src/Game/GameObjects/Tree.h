#pragma once
#ifndef TREE_H
#define TREE_H

#include <memory>

#include "Block.h"

#include "../../Render/Sprite2D.h"

namespace BatleCity
{
	class Tree : public Block
	{
	public:

		Tree(const EBlockType tree_type, const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer);

	private:

		void renderBlock(const ESubBlockLocation subBlock_location) const override;



		std::shared_ptr<RenderEngine::Sprite2D> m_sprite;

	};
}

#endif // !TREE_H
