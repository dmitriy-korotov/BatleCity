#include "IGameState.h"

namespace BatleCity
{
	IGameState::IGameState(EGameStates game_state)
			: m_game_state(game_state)
	{ }

	IGameState::EGameStates IGameState::getGameStateType() const noexcept
	{
		return m_game_state;
	}
}