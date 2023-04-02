#include "IGameState.h"

namespace BatleCity
{
	IGameState::IGameState(EGameStates game_state)
			: m_game_state(game_state)
	{ }
}