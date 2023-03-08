#include "AnimatedSprite2D.h"
#include "Texture2D.h"

#include <iostream>

namespace RenderEngine
{
	AnimatedSprite2D::AnimatedSprite2D(const std::shared_ptr<Texture2D> ptr_texture,
									   const std::shared_ptr<ShaderProgram> ptr_shader_program,
									   const std::string& subTexture_name) : Sprite2D(ptr_texture, ptr_shader_program, subTexture_name),
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



	void AnimatedSprite2D::render(const glm::vec2& position, const glm::vec2& size, const float rotation) const
	{
		if (m_need_rerender)
		{
			RenderEngine::Texture2D::SubTexture2D current_subTexture = m_texture->getSubTexture(m_current_state->second[m_current_frame].first);

			GLfloat texture_coords[] = {
			//					U										V
				current_subTexture.left_bottom_uv.x,	current_subTexture.left_bottom_uv.y,
				current_subTexture.left_bottom_uv.x,	current_subTexture.right_top_uv.y,			
				current_subTexture.right_top_uv.x,		current_subTexture.right_top_uv.y,
				current_subTexture.right_top_uv.x,		current_subTexture.left_bottom_uv.y,		
			};

			//					2 - amount coords in one point;		4 - amount points
			m_texture_coords_buffer.update(texture_coords, 2 * 4 * sizeof(GLfloat));
			m_need_rerender = false;
		}

		Sprite2D::render(position, size, rotation);
	}
}