#pragma once
#ifndef TANK_H
#define TANK_H

#include <iostream>
#include <glm/vec2.hpp>

#include "IGameObject.h"

#include "../../Render/SpriteAnimator.h"

namespace RenderEngine
{
	class Sprite2D;
	class SpriteAnimator;
}

namespace BatleCity
{
	class Tank : public IGameObject
	{
	public:

		enum class EOrientation
		{
			Top,
			Right,
			Bottom,
			Left
		};

		Tank(std::shared_ptr<RenderEngine::Sprite2D> sprite_ptr, const glm::vec2& position, const glm::vec2& size, const float velocity);

		void setOrientation(const EOrientation orientation);
		void update(const uint64_t delta) override;
		void render() const override;
		void move(const bool move);

	private:

		RenderEngine::SpriteAnimator m_sprite;
		
		EOrientation m_current_orientation;
		float m_velocity;
		bool m_move;
		glm::vec2 m_move_offset;

	};
}

#endif // !TANK_H
