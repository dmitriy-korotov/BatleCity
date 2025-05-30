#include "Game.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec2.hpp>

#include "GameStates/Level.h"
#include "GameStates/StartScreen.h"

#include "../Resources/ResourceManager.h"

#include <BatleCity/Physics/PhysicsEngine.h>
#include <BatleCity/Render/Renderer.h>

#include "../System/Window.h"

#include <GLFW/glfw3.h>

#include <iostream>
#include <memory>

namespace BatleCity {
Game& Game::Instance() {
  static Game game;
  return game;
}

Game::Game() {
  m_keys.fill(false);
  m_renderer = std::make_shared<RenderEngine::Renderer>();
  m_resources_manager = std::make_shared<Resources::ResourceManager>();
}

std::shared_ptr<RenderEngine::Renderer> Game::GetRenderer() {
  return m_renderer;
}

std::shared_ptr<Resources::ResourceManager> Game::GetResourcesManager() {
  return m_resources_manager;
}

bool Game::StartOn(std::shared_ptr<System::Window> window) {
  if (m_window_ptr) {
    std::cerr << "ERROR: Game already initializated" << std::endl;
    return false;
  }
  m_window_ptr = std::move(window);

  if (!gladLoadGL()) {
    std::cout << "ERROR: Can't load GLAD" << std::endl;
    return false;
  }

  std::cout << "Render: " << m_renderer->GetStringOpenGL(GL_RENDERER)
            << std::endl;
  std::cout << "OpenGL version: " << m_renderer->GetStringOpenGL(GL_VERSION)
            << "\n\n";

  if (!m_resources_manager->LoadAllResourcesJSON("res/resources.json")) {
    std::cerr
        << "ERROR: => Can't load all resources from JSON:\tres/resources.json"
        << std::endl;
    return false;
  }

  m_start_screen = m_resources_manager->GetStartScreen("StartScreen1");
  if (m_start_screen == nullptr) {
    std::cerr << "ERROR: Can't load start screen" << std::endl;
    return false;
  }

  m_level = m_resources_manager->GetLevel("Level2");
  if (m_level == nullptr) {
    std::cerr << "ERROR: Can't load level" << std::endl;
    return false;
  }

  m_current_game_state = m_start_screen;
  if (!m_current_game_state->start()) {
    return false;
  }

  m_renderer->SetClearColor(0.f, 0.f, 0.f);
  m_renderer->SetDepthTest(true);

  Physics::PhysicsEngine::init();

  return true;
}

void Game::Finish() {
  m_window_ptr.reset();
  m_start_screen.reset();
  m_level.reset();
  m_current_game_state.reset();

  m_resources_manager->UnloadAllResources();
  Physics::PhysicsEngine::Terminate();
}

void Game::SetKey(const int key, const int action) { m_keys[key] = action; }

void Game::ResetWindowSizeToCurrentGameState() noexcept {
  m_window_ptr->CallResizeCallBack(m_window_ptr->GetWindowWidth(),
                                   m_window_ptr->GetWindowHeight());
}

void Game::Update(const double delta) {
  if (m_current_game_state->getGameStateType() ==
      IGameState::EGameStates::StartScreen) {
    if (m_keys[GLFW_KEY_ENTER]) {
      switch (std::static_pointer_cast<StartScreen>(m_current_game_state)
                  ->select()) {
        case StartScreen::EMenuPuncts::LevelTwoPlayers:
          m_level->setLevelType(Level::ELevelType::TwoPlayers);
          m_current_game_state = m_level;
          m_current_game_state->start();
          break;
        case StartScreen::EMenuPuncts::LevelOnePlayer:
          m_level->setLevelType(Level::ELevelType::OnePlayer);
          m_current_game_state = m_level;
          m_current_game_state->start();
          break;
        case StartScreen::EMenuPuncts::Constructor:
          std::cout << "Constructor is not implemented yet" << std::endl;
      }
      ResetWindowSizeToCurrentGameState();
    }
  }
  if (m_current_game_state->getGameStateType() ==
      IGameState::EGameStates::Level) {
    if (m_keys[GLFW_KEY_Q]) {
      m_current_game_state = m_start_screen;
      m_current_game_state->start();
      ResetWindowSizeToCurrentGameState();
    }
  }
  m_current_game_state->update(delta, m_keys);
}

void Game::Render() {
  m_renderer->Clear(GL_COLOR_BUFFER_BIT);
  m_renderer->Clear(GL_DEPTH_BUFFER_BIT);
  m_current_game_state->render();
}

size_t Game::GetCurrentGameWidth() const {
  return m_current_game_state->getGameStateWidth();
}

size_t Game::GetCurrentGameHeight() const {
  return m_current_game_state->getGameStateHeight();
}
}  // namespace BatleCity