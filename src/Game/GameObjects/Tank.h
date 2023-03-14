#pragma once
#ifndef TANK_H
#define TANK_H

#include <iostream>
#include <glm/vec2.hpp>

#include "IGameObject.h"

#include "../../Render/SpriteAnimator.h"

#include "../../System/Timer.h"

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

		Tank(std::shared_ptr<RenderEngine::Sprite2D> sprite_ptr, const glm::vec2& position, const glm::vec2& size,
		     const double velocity, const float layer = 0.f);

		void setOrientation(const EOrientation orientation);
		void update(const double delta) override;
		void render() const override;
		void move(const bool move);

	private:

		RenderEngine::SpriteAnimator m_tank_sprite;
		std::pair<RenderEngine::SpriteAnimator, my_system::Timer> m_respawn_animation;
		std::pair<RenderEngine::SpriteAnimator, my_system::Timer> m_shield_animation;
		
		EOrientation m_current_orientation;
		double m_velocity;
		bool m_move;
		bool m_is_respawn;
		bool m_has_shild;
		glm::vec2 m_move_offset;

	};
}

#endif // !TANK_H
