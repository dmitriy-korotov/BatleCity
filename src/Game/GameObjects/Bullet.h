#pragma once
#ifndef BULLET_H
#define BULLET_H

#include <memory>

#include "IDynamicGameObject.h"

#include "../../Render/SpriteAnimator.h"

#include "../../System/Timer.h"

namespace BatleCity
{
	class Bullet : public IDynamicGameObject
	{
	public:

		Bullet(EOrientation bullet_type, const glm::vec2& size, float layer, double max_velocity);

		void fire(const glm::vec2& position, const glm::vec2& direction, double velocity);

		void update(double delta) override;
		void render() const override;

		bool onCollision(EGameObjectType game_object_type, std::shared_ptr<Physics::AABB> target_collider, const glm::vec2& direction = glm::vec2(0.f)) override;

	private:

		std::shared_ptr<RenderEngine::Sprite2D> m_sprite = nullptr;
		std::pair<RenderEngine::SpriteAnimator, my_system::Timer> m_explosion_animation;
		bool m_is_fire = false;
		bool m_is_exploded = false;

	};
}

#endif // !BULLET_H
