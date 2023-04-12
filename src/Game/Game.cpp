#include "Game.h"

#include "GameStates/Level.h"

#include "../Resources/ResourceManager.h"

#include "../Render/ShaderProgram.h"
#include "../Render/Texture2D.h"
#include "../Render/Sprite2D.h"
#include "../Render/AnimatedSprite2D.h"

#include "../Physics/PhysicsEngine.h"

#include "GameObjects/Tank.h"

#include <GLFW/glfw3.h>

#include <iostream>

#include <glm/vec2.hpp>



namespace BatleCity
{
	Game::Game() 								   
	{ 
		m_keys.fill(false);
	}



	Game::~Game()
	{ }



	bool Game::init()
	{
        if (!Resources::ResourceManager::loadResourcesJSON("res/resources.json"))
        {
            std::cerr << "ERROR: => Can't load all resources from JSON:\tres/resources.json" << std::endl;
            return false;
        }

 

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
        Physics::PhysicsEngine::setCurrentLevel(m_level);



        m_current_game_state = reinterpret_cast<std::shared_ptr<IGameState>&>(m_start_screen);
        if (!m_current_game_state->start())
        {
            return false;
        }

        return true;
	}



	void Game::setKey(const int key, const int action)
	{
        m_keys[key] = action;
	}



	void Game::update(const double delta)
	{
        if (m_current_game_state->getGameStateType() == IGameState::EGameStates::StartScreen)
        {
            if (m_keys[GLFW_KEY_ENTER])
            {
                m_current_game_state = reinterpret_cast<std::shared_ptr<Level>&>(m_level);
                m_current_game_state->start();
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