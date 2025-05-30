#pragma once
#ifndef GAME_H
#define GAME_H

#include <array>
#include <memory>

namespace System {
class Window;
}

namespace RenderEngine {
class ShaderProgram;
class AnimatedSprite2D;
class Renderer;
}  // namespace RenderEngine

namespace BatleCity {
class Tank;
class IGameState;
class Level;
class StartScreen;

class Game final {
 public:
  static Game& Instance();

  std::shared_ptr<RenderEngine::Renderer> GetRenderer();

  bool StartOn(std::shared_ptr<System::Window> window);
  void Finish();
  void SetKey(const int key, const int action);
  void Update(const double delta);
  void Render();

  size_t GetCurrentGameWidth() const;
  size_t GetCurrentGameHeight() const;

 private:
  explicit Game();

  void ResetWindowSizeToCurrentGameState() noexcept;

 private:
  std::shared_ptr<System::Window> m_window_ptr = nullptr;

  std::array<bool, 349> m_keys;

  std::shared_ptr<StartScreen> m_start_screen = nullptr;
  std::shared_ptr<Level> m_level = nullptr;

  std::shared_ptr<IGameState> m_current_game_state = nullptr;

  std::shared_ptr<RenderEngine::Renderer> m_renderer = nullptr;
};
}  // namespace BatleCity

#endif  // !GAME_H
