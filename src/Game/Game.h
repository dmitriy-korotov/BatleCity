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
	class IGameState;
	class Level;
	class StartScreen;

	class Game
	{
	public:

		Game();
		~Game();

		bool init();
		void setKey(const int key, const int action);
		void update(const double delta);
		void render();

		size_t getCurrentGameWidth() const;
		size_t getCurrentGameHeight() const;

	private:

		std::array<bool, 349> m_keys;

		//std::shared_ptr<StartScreen> m_start_screen;
		//std::shared_ptr<Level> m_level;

		std::shared_ptr<IGameState> m_current_game_state;

		//std::shared_ptr<Tank> m_tank;
		//std::shared_ptr<Tank> m_tank2;
		//std::shared_ptr<Level> m_level;

	};
}

#endif // !GAME_H
