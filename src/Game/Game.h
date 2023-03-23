#pragma once
#ifndef GAME_H
#define GAME_H

#include <glm/vec2.hpp>
#include <memory>
#include <array>

namespace RenderEngine
{
	class AnimatedSprite2D;
}

namespace BatleCity
{
	class Tank;
	class Level;

	class Game
	{
	public:

		Game(const glm::vec2& window_size);
		~Game();

		bool init();
		void setKey(const int key, const int action);
		void update(const double delta);
		void render();

		size_t getCurrentLevelWidth() const;
		size_t getCurrentLevelHeight() const;

	private:

		enum class EGameStates
		{
			Active,
			Pause
		};

		std::array<bool, 349> m_keys;

		glm::vec2 m_window_size;

		EGameStates m_current_game_state;

		std::shared_ptr<Tank> m_tank;
		std::shared_ptr<Tank> m_tank2;
		std::shared_ptr<Level> m_level;

	};
}

#endif // !GAME_H
