#include "SpriteAnimator.h"

#include "Texture2D.h"
#include "../Resources/ResourceManager.h"

#include <iostream>

namespace RenderEngine
{
	SpriteAnimator::SpriteAnimator(std::shared_ptr<Sprite2D> sprite_ptr) : m_animated_sprite(sprite_ptr), m_currnet_sprite(std::move(sprite_ptr))
	{ 
		m_current_state = m_animated_sprite->m_states.end();
	}



	void SpriteAnimator::setState(const std::string& state_name)
	{
		Sprite2D::MapStates::const_iterator state = m_animated_sprite->m_states.find(state_name);
		if (state == m_animated_sprite->m_states.end())
		{
			std::cerr << "Can't find state: " << state_name << std::endl;
			return;
		}
		if (state != m_current_state)
		{
			m_current_state = state;
			m_current_frame = 0;
			m_current_time_animation = 0;
			m_is_need_rerender = true;
		}
	}




	void SpriteAnimator::update(const uint64_t delta)
	{
		if (m_current_state != m_animated_sprite->m_states.end())
		{
			m_current_time_animation += delta;

			while (m_current_time_animation >= m_current_state->second[m_current_frame].second)
			{
				m_current_time_animation -= m_current_state->second[m_current_frame].second;
				++m_current_frame;
				m_is_need_rerender = true;

				if (m_current_frame >= m_current_state->second.size())
				{
					m_current_frame = 0;
				}
			}
		}
	}



	void SpriteAnimator::render(const glm::vec2& position, const glm::vec2& size, const float rotation) const
	{
		if (m_is_need_rerender)
		{
			m_currnet_sprite = Resources::ResourceManager::getSprite(m_current_state->second[m_current_frame].first);
			m_is_need_rerender = false;
		}
		m_currnet_sprite->render(position, size, rotation);
	}
}