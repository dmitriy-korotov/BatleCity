#pragma once
#ifndef SPRITE_ANIMATOR_H
#define SPRITE_ANIMATOR_H

#include <glm/vec2.hpp>

#include <map>
#include <vector>
#include <string>

#include "Sprite2D.h"

namespace RenderEngine
{
	class SpriteAnimator
	{
	public:

		SpriteAnimator(std::shared_ptr<Sprite2D> sprite_ptr);

		void setState(const std::string& state_name);
		void update(const uint64_t delta);
		void render(const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer) const;

	private:

		std::shared_ptr<Sprite2D> m_animated_sprite;
		mutable std::shared_ptr<Sprite2D> m_currnet_sprite;

		Sprite2D::MapStates::const_iterator m_current_state;
		uint64_t m_current_time_animation = 0;
		unsigned int m_current_frame = 0;

		mutable bool m_is_need_rerender = false;

	};
}

#endif // !SPRITE_ANIMATOR_H
