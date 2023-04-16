#pragma once
#ifndef TANK_H
#define TANK_H

#include <glm/vec2.hpp>

#include "../GameObjectContainers/BulletCollector.h"



namespace my_system
{
	class Timer;
}

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
		
		enum class ETankType : uint8_t
		{
			YellowTank1 = 0,
			YellowTank2,
			YellowTank3,
			YellowTank4,

			WhiteTank1,
			WhiteTank2,
			WhiteTank3,
			WhiteTank4,

			GreenTank1,
			GreenTank2,
			GreenTank3,
			GreenTank4,
			GreenTank5,
			GreenTank6,
			GreenTank7,
			GreenTank8,

			RedTank1,
			RedTank2,
			RedTank3,
			RedTank4
		};

		Tank(ETankType tank_type, const glm::vec2& position, const glm::vec2& size,
			 double max_velocity, double delay_between_shots = 200, const glm::vec2& direction = glm::vec2(0.f, 1.f),
			 double velocity = 0, float layer = 0.f);

		void setVelocity(double velocity) override;
		void setOrientation(const EOrientation orientation) override;

		void update(const double delta) override;
		void render() const override;

		void fair() const;
		bool onCollision(EGameObjectType game_object_type, std::shared_ptr<Physics::AABB> target_collider, const glm::vec2& direction = glm::vec2(0.f)) override;

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

		inline static std::vector<std::string> m_all_kinds_tank_sprites = 
												{ "yellowTank1AnimatedSprite", "yellowTank2AnimatedSprite",  "yellowTank3AnimatedSprite", "yellowTank4AnimatedSprite",
												  "whiteTank1AnimatedSprite",  "whiteTank2AnimatedSprite",   "whiteTank3AnimatedSprite",  "whiteTank4AnimatedSprite",
												  "greenTank1AnimatedSprite",  "greenTank3AnimatedSprite",   "greenTank4AnimatedSprite",  "greenTank4AnimatedSprite",
												  "greenTank5AnimatedSprite",  "greenTank6AnimatedSprite",   "greenTank7AnimatedSprite",  "greenTank8AnimatedSprite",
												  "redTank1AnimatedSprite",    "redTank2AnimatedSprite",     "redTank3AnimatedSprite",    "redTank4AnimatedSprite" };

	};
}

#endif // !TANK_H
