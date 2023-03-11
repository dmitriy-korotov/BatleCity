#pragma once
#ifndef BORDER_H
#define BORDER_H

#include <memory>

#include "Block.h"

#include "../../Render/SpriteAnimator.h"

namespace BatleCity
{
	class Border : public Block
	{
	public:

		Border(const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer);

		void render() const override;

	private:

		std::shared_ptr<RenderEngine::Sprite2D> m_sprite;

	};
}

#endif // !BORDER_H