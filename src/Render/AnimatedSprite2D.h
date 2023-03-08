#pragma once
#ifndef ANIMATED_SPRITE_H
#define ANIMATED_SPRITE_H

#include <map>
#include <vector>

#include "Sprite2D.h"

namespace RenderEngine
{
	class AnimatedSprite2D : public Sprite2D
	{
	public:

		AnimatedSprite2D(const std::shared_ptr<Texture2D> ptr_texture,
						 const std::shared_ptr<ShaderProgram> ptr_shader_program,
						 const std::string& subTexture_name = "default");

		void addState(const std::string& state_name, std::vector<std::pair<std::string, uint64_t>> frames);
		void setState(const std::string& state_name);
		void update(const uint64_t delta);
		void render(const glm::vec2& position, const glm::vec2& size, const float rotation) const override;

	private:

		typedef std::map<std::string, std::vector<std::pair<std::string, uint64_t>>> MapStates;
		MapStates m_states;

		MapStates::const_iterator m_current_state;

		size_t m_current_frame;
		uint64_t m_current_time_animation;

		mutable bool m_need_rerender;
	};
}

#endif // ANIMATED_SPRITE_H