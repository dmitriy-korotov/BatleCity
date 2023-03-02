#include "AnimatedSprite2D.h"
#include "Texture2D.h"

#include <iostream>

namespace Renderer
{
	AnimatedSprite2D::AnimatedSprite2D(const std::shared_ptr<Texture2D> ptr_texture,
									   const std::shared_ptr<ShaderProgram> ptr_shader_program,
									   const std::string& subTexture_name,
									   const glm::vec2& position,
									   const unsigned int sprite_width,
									   const unsigned int sprite_height,
									   const float angle_rotation) : Sprite2D(ptr_texture, ptr_shader_program,
																			  subTexture_name, position,
																		      sprite_width, sprite_height, angle_rotation),
																	  m_current_frame(0),
																	  m_current_time_animation(0),
																	  m_need_rerender(false)
	{
		m_current_state = m_states.end();
	}



	void AnimatedSprite2D::addState(const std::string& state_name, std::vector<std::pair<std::string, uint64_t>> frames)
	{
		m_states.emplace(state_name, std::move(frames));
	}



	void AnimatedSprite2D::setState(const std::string& state_name)
	{
		MapStates::const_iterator state = m_states.find(state_name);
		if (state == m_states.end())
		{
			std::cerr << "Can't find state: " << state_name << std::endl;
			return;
		}
		if (state != m_current_state)
		{
			m_current_state = state;
			m_current_frame = 0;
			m_current_time_animation = 0;
			m_need_rerender = true;
		}
	}



	void AnimatedSprite2D::update(const uint64_t delta) 
	{
		if (m_current_state != m_states.end())
		{
			m_current_time_animation += delta;

			while (m_current_time_animation >= m_current_state->second[m_current_frame].second)
			{
				m_current_time_animation -= m_current_state->second[m_current_frame].second;
				++m_current_frame;
				m_need_rerender = true;

				if (m_current_frame >= m_current_state->second.size())
				{
					m_current_frame = 0;
				}
			}
		}
	}



	void AnimatedSprite2D::render() const
	{
		if (m_need_rerender)
		{
			Renderer::Texture2D::SubTexture2D current_subTexture = m_texture->getSubTexture(m_current_state->second[m_current_frame].first);

			GLfloat texture_coords[] = {
			//					U										V
				current_subTexture.left_bottom_uv.x,	current_subTexture.left_bottom_uv.y,
				current_subTexture.left_bottom_uv.x,	current_subTexture.right_top_uv.y,			// first triangle
				current_subTexture.right_top_uv.x,		current_subTexture.right_top_uv.y,

				current_subTexture.right_top_uv.x,		current_subTexture.right_top_uv.y,
				current_subTexture.right_top_uv.x,		current_subTexture.left_bottom_uv.y,			// second triangle
				current_subTexture.left_bottom_uv.x,	current_subTexture.left_bottom_uv.y,
			};

			glBindBuffer(GL_ARRAY_BUFFER, m_tex_coords_vbo);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(texture_coords), &texture_coords);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			m_need_rerender = false;
		}

		Sprite2D::render();
	}
}