#pragma once
#ifndef I_GAME_STATE_H
#define I_GAME_STATE_H

#include <array>



namespace BatleCity
{
	class IGameState
	{
	public:

		enum class EGameStates
		{
			StartScreen,
			Level,
			Pause,
			EndScreen
		};

		explicit IGameState(EGameStates game_state);
		virtual ~IGameState() { };
		virtual size_t getGameStateWidth() const noexcept = 0;
		virtual size_t getGameStateHeight() const noexcept = 0;
		virtual bool start() const noexcept = 0;
		virtual void update(const double delta, std::array<bool, 349>& keyboard) = 0;
		virtual void render() const = 0;

		EGameStates getGameStateType() const noexcept;

	protected:

		EGameStates m_game_state = EGameStates::StartScreen;

		unsigned int m_width_blocks = 0;
		unsigned int m_height_blocks = 0;
		unsigned int m_width_pixels = 0;
		unsigned int m_height_pixels = 0;

	};
}

#endif // !I_GAME_STATE_H
