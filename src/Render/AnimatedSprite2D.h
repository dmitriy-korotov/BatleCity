#pragma once
#ifndef ANIMATED_SPRITE_H
#define ANIMATED_SPRITE_H

#include <map>
#include <vector>

#include "Sprite2D.h"

namespace Renderer
{
	//class Texture2D;
	//class ShaderProgram;

	class AnimatedSprite2D : public Sprite2D
	{
	public:

		AnimatedSprite2D(const std::shared_ptr<Texture2D> ptr_texture,
						 const std::shared_ptr<ShaderProgram> ptr_shader_program,
						 const std::string& subTexture_name = "default",
						 const glm::vec2 position = glm::vec2(0.f, 0.f),
						 const unsigned int sprite_hidth = 1,
						 const unsigned int sprite_height = 1,
						 const float angle_rotation = 0);

		void addState(const std::string& state_name, std::vector<std::pair<std::string, uint64_t>> frames);

		void setState(const std::string& state_name);

		void update(const uint64_t delta);

		void render() const override;

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