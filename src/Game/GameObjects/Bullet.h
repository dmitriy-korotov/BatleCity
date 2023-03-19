#pragma once
#ifndef BULLET_H
#define BULLET_H

#include <memory>

#include "IDynamicGameObject.h"

#include "../../Render/SpriteAnimator.h"

namespace BatleCity
{
	class Bullet : public IDynamicGameObject
	{
	public:

		Bullet(EOrientation bullet_type, const glm::vec2& size, float layer, double max_velocity);

		void fire(const glm::vec2& position, const glm::vec2& direction, double velocity);

		void update(double delta) override;
		void render() const override;

		double getMaxVelocity() const { return m_max_velocity; }

		void onCollision() override;

	private:

		std::shared_ptr<RenderEngine::Sprite2D> m_sprite = nullptr;
		double m_max_velocity = 0;
		bool m_is_fire = false;

	};
}

#endif // !BULLET_H
