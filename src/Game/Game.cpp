#include "Game.h"

#include <glm/vec2.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "GameStates/Level.h"
#include "GameStates/StartScreen.h"

#include "../Resources/ResourceManager.h"

#include "../Render/ShaderProgram.h"

#include "../Physics/PhysicsEngine.h"

#include "../System/Window.h"

#include <iostream>

#include <GLFW/glfw3.h>


namespace BatleCity
{
	Game::Game() 								   
	{ 
		m_keys.fill(false);
	}



	Game::~Game()
	{ }



	bool Game::init(std::shared_ptr<my_system::Window> window_ptr)
	{
        if (!m_window_ptr)
        {
            if (!Resources::ResourceManager::loadAllResourcesJSON("res/resources.json"))
            {
                std::cerr << "ERROR: => Can't load all resources from JSON:\tres/resources.json" << std::endl;
                return false;
            }
            m_window_ptr = std::move(window_ptr);


            m_start_screen = Resources::ResourceManager::getStartScreen("StartScreen1");
            if (m_start_screen == nullptr)
            {
                std::cerr << "ERROR: Can't load start screen" << std::endl;
                return false;
            }

            m_level = Resources::ResourceManager::getLevel("Level6");
            if (m_level == nullptr)
            {
                std::cerr << "ERROR: Can't load level" << std::endl;
            }


            m_current_game_state = reinterpret_cast<std::shared_ptr<IGameState>&>(m_start_screen);
            if (!m_current_game_state->start())
            {
                return false;
            }

            return true;
        }
        else
        {
            std::cerr << "ERROR: Game already initializated" << std::endl;
            return false;
        }
	}



	void Game::setKey(const int key, const int action)
	{
        m_keys[key] = action;
	}



    void Game::resetWindowSizeToCurrentGameState() noexcept
    {
        m_window_ptr->callResizeCallBack(m_window_ptr->getWindowWidth(), m_window_ptr->getWindowHeight());
    }



	void Game::update(const double delta)
	{
        if (m_current_game_state->getGameStateType() == IGameState::EGameStates::StartScreen)
        {
            if (m_keys[GLFW_KEY_ENTER])
            {
                switch (reinterpret_cast<std::shared_ptr<StartScreen>&>(m_current_game_state)->select())
                {
                case StartScreen::EMenuPuncts::LevelTwoPlayers:
                    m_current_game_state = m_level;
                    m_current_game_state->start();
                    resetWindowSizeToCurrentGameState();
                    break;
                }
            }
        }
        if (m_current_game_state->getGameStateType() == IGameState::EGameStates::Level)
        {
            if (m_keys[GLFW_KEY_Q])
            {
                m_current_game_state = reinterpret_cast<std::shared_ptr<IGameState>&>(m_start_screen);
                m_current_game_state->start();
                resetWindowSizeToCurrentGameState();
            }
        }
        m_current_game_state->update(delta, m_keys);
	}



	void Game::render()
	{
        m_current_game_state->render();
	}



    size_t Game::getCurrentGameWidth() const
    {
        return m_current_game_state->getGameStateWidth();
    }



    size_t Game::getCurrentGameHeight() const
    {
        return m_current_game_state->getGameStateHeight();
    }
}