#include "Game.h"
#include "Level.h"

#include "../Resources/ResourceManager.h"

#include "../Render/ShaderProgram.h"
#include "../Render/Texture2D.h"
#include "../Render/Sprite2D.h"
#include "../Render/AnimatedSprite2D.h"

#include "../Physics/PhysicsEngine.h"

#include "GameObjects/Tank.h"

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
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

        std::shared_ptr<RenderEngine::ShaderProgram> shader_program = Resources::ResourceManager::getShaderProgram("spriteShaderProgram");
        if (shader_program == nullptr)
        {
            std::cerr << "Can't load shader program " << "spriteShaderProgram" << std::endl;
            return false;
        }

        std::shared_ptr<RenderEngine::Sprite2D> tank_sprite = Resources::ResourceManager::getSprite("Border_8x8");
        if (tank_sprite == nullptr)
        {
            std::cerr << "Can't find animatad sprite: " << "yellowTankAnimatedSprite" << std::endl;
            return false;
        }

        m_level = std::make_shared<Level>(std::move(*Resources::ResourceManager::getLevel(3)));

        m_tank = std::make_shared<Tank>(tank_sprite, m_level->getPlayer1Respawn(), glm::vec2(Level::BLOCK_SIZE, Level::BLOCK_SIZE), 0.05f);
        m_tank2 = std::make_shared<Tank>(std::move(tank_sprite), m_level->getPlayer2Respawn(), glm::vec2(Level::BLOCK_SIZE, Level::BLOCK_SIZE), 0.05f);

        Physics::PhysicsEngine::addDynamicGameObject(m_tank);
        Physics::PhysicsEngine::addDynamicGameObject(m_tank2);
        Physics::PhysicsEngine::setCurrentLevel(m_level);

        glm::mat4 projection_matrix = glm::ortho<float>(0.f, m_level->getLevelWidth(), 0.f, m_level->getLevelHeight(), -100.f, 100.f);
        shader_program->use();
        shader_program->setMatrix4("clip_matrix", projection_matrix);


        return true;
	}



	void Game::setKey(const int key, const int action)
	{
        m_keys[key] = action;
	}

	void Game::update(const double delta)
	{
       if (m_level)
       {
           m_level->update(delta);
       }
       if (m_tank)
       {
           if (m_tank)
           {
               if (m_keys[GLFW_KEY_W])
               {
                   m_tank->setOrientation(Tank::EOrientation::Top);
                   m_tank->setVelocity(m_tank->getMaxVelocity());
               }
               else if (m_keys[GLFW_KEY_D])
               {
                   m_tank->setOrientation(Tank::EOrientation::Right);
                   m_tank->setVelocity(m_tank->getMaxVelocity());
               }
               else if (m_keys[GLFW_KEY_S])
               {
                   m_tank->setOrientation(Tank::EOrientation::Bottom);
                   m_tank->setVelocity(m_tank->getMaxVelocity());
               }
               else if (m_keys[GLFW_KEY_A])
               {
                   m_tank->setOrientation(Tank::EOrientation::Left);
                   m_tank->setVelocity(m_tank->getMaxVelocity());
               }
               else
               {
                   m_tank->setVelocity(0);
               }
           }
           m_tank->update(delta);
       }

       if (m_tank2)
       {
           if (m_tank2)
           {
               if (m_keys[GLFW_KEY_UP])
               {
                   m_tank2->setOrientation(Tank::EOrientation::Top);
                   m_tank2->setVelocity(m_tank->getMaxVelocity());
               }
               else if (m_keys[GLFW_KEY_RIGHT])
               {
                   m_tank2->setOrientation(Tank::EOrientation::Right);
                   m_tank2->setVelocity(m_tank->getMaxVelocity());
               }
               else if (m_keys[GLFW_KEY_DOWN])
               {
                   m_tank2->setOrientation(Tank::EOrientation::Bottom);
                   m_tank2->setVelocity(m_tank->getMaxVelocity());
               }
               else if (m_keys[GLFW_KEY_LEFT])
               {
                   m_tank2->setOrientation(Tank::EOrientation::Left);
                   m_tank2->setVelocity(m_tank->getMaxVelocity());
               }
               else
               {
                   m_tank2->setVelocity(0);
               }
           }
           m_tank2->update(delta);
       }
       Physics::PhysicsEngine::update(delta);
	}



	void Game::render()
	{
        if (m_tank)
        {
            m_tank->render();
        }
        if (m_tank2)
        {
            m_tank2->render();
        }
        if (m_level)
        {
            m_level->render();
        }
	}



    size_t Game::getCurrentLevelWidth() const
    {
        return m_level->getLevelWidth();
    }



    size_t Game::getCurrentLevelHeight() const
    {
        return m_level->getLevelHeight();
    }
}