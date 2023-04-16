#pragma once
#ifndef GAME_H
#define GAME_H

#include <memory>
#include <array>



namespace my_system
{
	class Window;
}

namespace RenderEngine
{
	class ShaderProgram;
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

		bool init(std::shared_ptr<my_system::Window> window_ptr);
		void setKey(const int key, const int action);
		void update(const double delta);
		void render();

		size_t getCurrentGameWidth() const;
		size_t getCurrentGameHeight() const;

	private:

		void resetWindowSizeToCurrentGameState() noexcept;



		std::shared_ptr<my_system::Window> m_window_ptr = nullptr;

		std::array<bool, 349> m_keys;

		std::shared_ptr<StartScreen> m_start_screen = nullptr;
		std::shared_ptr<Level> m_level = nullptr;

		std::shared_ptr<IGameState> m_current_game_state = nullptr;

	};
}

#endif // !GAME_H
