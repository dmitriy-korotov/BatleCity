#include "Game.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec2.hpp>

#include "GameStates/Level.h"
#include "GameStates/StartScreen.h"

#include <BatleCity/Physics/PhysicsEngine.h>
#include <BatleCity/Render/Renderer.h>
#include <BatleCity/Resources/ResourceManager.h>

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
  m_resourcesManager = std::make_shared<Resources::ResourceManager>();
  m_physicsEngine = std::make_shared<Physics::PhysicsEngine>();
}

std::shared_ptr<RenderEngine::Renderer> Game::GetRenderer() {
  return m_renderer;
}

std::shared_ptr<Resources::ResourceManager> Game::GetResourcesManager() {
  return m_resourcesManager;
}

std::shared_ptr<Physics::PhysicsEngine> Game::GetPhysicsEngine() {
  return m_physicsEngine;
}

bool Game::StartOn(std::shared_ptr<System::Window> window) {
  if (m_window) {
    std::cerr << "ERROR: Game already initializated" << std::endl;
    return false;
  }
  m_window = std::move(window);

  if (!gladLoadGL()) {
    std::cout << "ERROR: Can't load GLAD" << std::endl;
    return false;
  }

  std::cout << "Render: " << m_renderer->GetStringOpenGL(GL_RENDERER)
            << std::endl;
  std::cout << "OpenGL version: " << m_renderer->GetStringOpenGL(GL_VERSION)
            << "\n\n";

  if (!m_resourcesManager->LoadAllResourcesJSON("res/resources.json")) {
    std::cerr
        << "ERROR: => Can't load all resources from JSON:\tres/resources.json"
        << std::endl;
    return false;
  }

  m_startScreen = m_resourcesManager->GetStartScreen("StartScreen1");
  if (m_startScreen == nullptr) {
    std::cerr << "ERROR: Can't load start screen" << std::endl;
    return false;
  }

  m_level = m_resourcesManager->GetLevel("Level2");
  if (m_level == nullptr) {
    std::cerr << "ERROR: Can't load level" << std::endl;
    return false;
  }

  m_currentGameState = m_startScreen;
  if (!m_currentGameState->start()) {
    return false;
  }

  m_renderer->SetClearColor(0.f, 0.f, 0.f);
  m_renderer->SetDepthTest(true);

  m_physicsEngine->Init();

  return true;
}

void Game::Finish() {
  m_window.reset();
  m_startScreen.reset();
  m_level.reset();
  m_currentGameState.reset();

  m_resourcesManager->UnloadAllResources();
  m_physicsEngine->Terminate();
}

void Game::SetKey(const int key, const int action) { m_keys[key] = action; }

void Game::ResetWindowSizeToCurrentGameState() noexcept {
  m_window->CallResizeCallBack(m_window->GetWindowWidth(),
                               m_window->GetWindowHeight());
}

void Game::Update(const double delta) {
  if (m_currentGameState->GetGameStateType() ==
      IGameState::EGameStates::StartScreen) {
    if (m_keys[GLFW_KEY_ENTER]) {
      switch (
          std::static_pointer_cast<StartScreen>(m_currentGameState)->select()) {
        case StartScreen::EMenuPuncts::LevelTwoPlayers:
          m_level->SetLevelType(Level::ELevelType::TwoPlayers);
          break;
        case StartScreen::EMenuPuncts::LevelOnePlayer:
          m_level->SetLevelType(Level::ELevelType::OnePlayer);
          break;
        case StartScreen::EMenuPuncts::Constructor:
          std::cout << "Constructor is not implemented yet" << std::endl;
      }
      m_currentGameState->Reset();
      m_currentGameState = m_level;
      m_currentGameState->start();
      ResetWindowSizeToCurrentGameState();
    }
  }
  if (m_currentGameState->GetGameStateType() ==
      IGameState::EGameStates::Level) {
    if (m_keys[GLFW_KEY_Q]) {
      m_currentGameState->Reset();
      m_currentGameState = m_startScreen;
      m_currentGameState->start();
      ResetWindowSizeToCurrentGameState();
    }
  }
  m_currentGameState->Update(delta, m_keys);
}

void Game::Render() {
  m_renderer->Clear(GL_COLOR_BUFFER_BIT);
  m_renderer->Clear(GL_DEPTH_BUFFER_BIT);
  m_currentGameState->Render();
}

size_t Game::GetCurrentGameWidth() const {
  return m_currentGameState->GetGameStateWidth();
}

size_t Game::GetCurrentGameHeight() const {
  return m_currentGameState->GetGameStateHeight();
}
}  // namespace BatleCity