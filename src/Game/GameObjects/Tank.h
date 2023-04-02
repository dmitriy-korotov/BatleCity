#pragma once
#ifndef TANK_H
#define TANK_H

#include <iostream>
#include <unordered_set>
#include <glm/vec2.hpp>

#include "IDynamicGameObject.h"

#include "../GameObjectContainers/BulletCollector.h"

#include "../../Render/SpriteAnimator.h"

#include "../../System/Timer.h"

namespace RenderEngine
{
	class Sprite2D;
	class SpriteAnimator;
}

namespace BatleCity
{
	class Tank : public IDynamicGameObject
	{
	public:

		Tank(std::shared_ptr<RenderEngine::Sprite2D> sprite_ptr, const glm::vec2& position, const glm::vec2& size,
			 double max_velocity, double delay_between_shots = 1000, const glm::vec2& direction = glm::vec2(0.f, 1.f),
			 double velocity = 0, float layer = 0.f);

		void setVelocity(double velocity) override;
		void setOrientation(const EOrientation orientation) override;

		void update(const double delta) override;
		void render() const override;

		void fair() const;
		bool onCollision(EGameObjectType game_object_type, const glm::vec2& direction = glm::vec2(0.f)) override;

	private:

		RenderEngine::SpriteAnimator m_tank_sprite;
		std::pair<RenderEngine::SpriteAnimator, my_system::Timer> m_respawn_animation;
		std::pair<RenderEngine::SpriteAnimator, my_system::Timer> m_shield_animation;
		
		mutable my_system::Timer m_timer_for_shots;
		mutable BulletCollector m_bullets;
		mutable bool m_is_fair = false;
		double m_delay_between_shots = 0;

		double m_min_velocity = 0;
		bool m_is_respawn = true;
		bool m_has_shild = false;

	};
}

#endif // !TANK_H
