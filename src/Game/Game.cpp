#include "Game.h"
#include "../Resources/ResourceManager.h"

#include "../Render/ShaderProgram.h"
#include "../Render/Texture2D.h"
#include "../Render/Sprite2D.h"
#include "../Render/AnimatedSprite2D.h"

#include "Tank.h"

#include <GLFW/glfw3.h>
#include <iostream>

namespace BatleCity
{
	Game::Game(const glm::vec2& window_size) : m_window_size(window_size),
											   m_current_game_state(Game::EGameStates::Active)
	{ 
		m_keys.fill(false);
	}



	Game::~Game()
	{ }



	bool Game::init()
	{
        if (!Resources::ResourceManager::loadResourcesJSON("res/resources.json"))
        {
            std::cerr << "Can't load all resources from JSON" << std::endl;
            return false;
        }

        /*std::shared_ptr<RenderEngine::ShaderProgram> shader_program_sprite = Resources::ResourceManager::getShaderProgram("spriteShaderProgram");
        if (shader_program_sprite == nullptr || !shader_program_sprite->isCompiled())
        {
            std::cerr << "Can't compiled shader program" << std::endl;
            return false;
        }*/

        std::shared_ptr<RenderEngine::AnimatedSprite2D> tank_sprite = Resources::ResourceManager::getAnimatedSprite("yellowTankAnimatedSprite");
        if (tank_sprite == nullptr)
        {
            std::cerr << "Can't find animatad sprite" << std::endl;
            return false;
        }

        m_water = Resources::ResourceManager::getAnimatedSprite("water");
        if (m_water == nullptr)
        {
            std::cerr << "Can't find animatad sprite" << std::endl;
            return false;
        }

        tank_sprite->setState("tankTopState");
        m_water->setState("default");
        m_water->setPosition(glm::vec2(300, 300));

        m_tank = std::make_unique<Tank>(tank_sprite, glm::vec2(0.f, 0.f), 0.0000001f);
		
        return true;
	}



	void Game::setKey(const int key, const int action)
	{
        m_keys[key] = action;
	}

	void Game::update(const uint64_t delta)
	{
       if (m_tank)
       {
           if (m_tank)
           {
               if (m_keys[GLFW_KEY_W])
               {
                   m_tank->setOrientation(Tank::EOrientation::Top);
                   m_tank->move(true);
               }
               else if (m_keys[GLFW_KEY_D])
               {
                   m_tank->setOrientation(Tank::EOrientation::Right);
                   m_tank->move(true);
               }
               else if (m_keys[GLFW_KEY_S])
               {
                   m_tank->setOrientation(Tank::EOrientation::Bottom);
                   m_tank->move(true);
               }
               else if (m_keys[GLFW_KEY_A])
               {
                   m_tank->setOrientation(Tank::EOrientation::Left);
                   m_tank->move(true);
               }
               else
               {
                   m_tank->move(false);
               }
           }
           m_tank->update(delta);
       }
       if (m_water)
       {
           m_water->update(delta);
       }
	}



	void Game::render()
	{
        if (m_tank)
        {
            m_tank->render();
        }
        if (m_water)
        {
            m_water->render();
        }
	}
}