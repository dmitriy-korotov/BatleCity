#pragma once
#ifndef TANK_H
#define TANK_H

#include <iostream>
#include "glm/vec2.hpp"

namespace Renderer
{
	class AnimatedSprite2D;
}

namespace batle_city
{
	class Tank
	{
	public:

		enum class EOrientation
		{
			Top,
			Right,
			Bottom,
			Left
		};

		Tank(std::shared_ptr<Renderer::AnimatedSprite2D> sprite, const glm::vec2& position, const float velocity);

		void setOrientation(const EOrientation orientation);

		void update(const uint64_t delta);

		void render() const;

		void move(const bool move);

	private:

		std::shared_ptr<Renderer::AnimatedSprite2D> m_sprite;
		
		EOrientation m_current_orientation;
		glm::vec2 m_position;
		float m_velocity;
		bool m_move;
		glm::vec2 m_move_offset;

	};
}

#endif // !TANK_H
