#pragma once
#ifndef TANK_H
#define TANK_H

#include <iostream>
#include <glm/vec2.hpp>

#include "IGameObject.h"

namespace RenderEngine
{
	class AnimatedSprite2D;
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

		Tank(std::shared_ptr<RenderEngine::AnimatedSprite2D> sprite, const glm::vec2& position, const glm::vec2& size, const float velocity);

		void setOrientation(const EOrientation orientation);

		void update(const uint64_t delta) override;

		void render() const override;

		void move(const bool move);

	private:

		std::shared_ptr<RenderEngine::AnimatedSprite2D> m_sprite;
		
		EOrientation m_current_orientation;
		float m_velocity;
		bool m_move;
		glm::vec2 m_move_offset;

	};
}

#endif // !TANK_H
