#pragma once
#ifndef GAME_H
#define GAME_H

#include <array>
#include <memory>

namespace System {
class Window;
}

namespace Physics {
class PhysicsEngine;
}

namespace Resources {
class ResourceManager;
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

using KeyboardType = std::array<bool, 349>;

class Game final {
 public:
  static Game& Instance();

  std::shared_ptr<RenderEngine::Renderer> GetRenderer();
  std::shared_ptr<Resources::ResourceManager> GetResourcesManager();
  std::shared_ptr<Physics::PhysicsEngine> GetPhysicsEngine();

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
  KeyboardType m_keys;

  std::shared_ptr<System::Window> m_window = nullptr;

  std::shared_ptr<Level> m_level = nullptr;
  std::shared_ptr<StartScreen> m_startScreen = nullptr;
  std::shared_ptr<IGameState> m_currentGameState = nullptr;

  std::shared_ptr<RenderEngine::Renderer> m_renderer = nullptr;
  std::shared_ptr<Resources::ResourceManager> m_resourcesManager = nullptr;
  std::shared_ptr<Physics::PhysicsEngine> m_physicsEngine = nullptr;
};
}  // namespace BatleCity

#endif  // !GAME_H
