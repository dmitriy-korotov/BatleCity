#pragma once
#ifndef TANK_H
#define TANK_H

#include <iostream>
#include <glm/vec2.hpp>

#include "IDynamicGameObject.h"

#include "../../Render/SpriteAnimator.h"

#include "../../System/Timer.h"

namespace RenderEngine
{
	class Sprite2D;
	class SpriteAnimator;
}

namespace BatleCity
{
	class Bullet;

	class Tank : public IDynamicGameObject
	{
	public:

		Tank(std::shared_ptr<RenderEngine::Sprite2D> sprite_ptr, const glm::vec2& position, const glm::vec2& size,
		     double max_velocity, const glm::vec2& direction = glm::vec2(0.f, 1.f), double velocity = 0, float layer = 0.f);

		void setVelocity(double velocity) override;
		void setOrientation(const EOrientation orientation);
		void update(const double delta) override;
		void render() const override;

		double getMaxVelocity() const;

		void fair() const;
		void onCollision() override;

	private:

		RenderEngine::SpriteAnimator m_tank_sprite;
		std::pair<RenderEngine::SpriteAnimator, my_system::Timer> m_respawn_animation;
		std::pair<RenderEngine::SpriteAnimator, my_system::Timer> m_shield_animation;
		
		mutable std::shared_ptr<Bullet> m_bullet = nullptr;
		mutable bool m_is_fair = false;

		EOrientation m_current_orientation;
		double m_max_velocity = 0;
		bool m_is_respawn = 0;
		bool m_has_shild = 0;

	};
}

#endif // !TANK_H
