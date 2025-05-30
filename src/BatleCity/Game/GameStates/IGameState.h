#pragma once
#ifndef I_GAME_STATE_H
#define I_GAME_STATE_H

#include <BatleCity/Game/Game.h>

namespace RenderEngine {
class ShaderProgram;
}

namespace BatleCity {
class IGameState {
 public:
  enum class EGameStates { StartScreen, Level, PaUse, EndScreen };

  explicit IGameState(EGameStates game_state);
  virtual ~IGameState(){};
  virtual size_t GetGameStateWidth() const noexcept = 0;
  virtual size_t GetGameStateHeight() const noexcept = 0;
  virtual bool start() const noexcept = 0;
  virtual void Reset() noexcept = 0;
  virtual void Update(const double delta, KeyboardType& keyboard) = 0;
  virtual void Render() const = 0;

  EGameStates GetGameStateType() const noexcept;

 protected:
  EGameStates m_game_state = EGameStates::StartScreen;

  unsigned int m_width_blocks = 0;
  unsigned int m_height_blocks = 0;
  unsigned int m_width_pixels = 0;
  unsigned int m_height_pixels = 0;
};
}  // namespace BatleCity

#endif  // !I_GAME_STATE_H
