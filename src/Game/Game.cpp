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
//#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

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

        /*std::shared_ptr<RenderEngine::ShaderProgram> shader_program_sprites = Resources::ResourceManager::getShaderProgram("spriteShaderProgram");
        if (shader_program_sprites == nullptr)
        {
            std::cerr << "Can't load shader program " << "spriteShaderProgram" << std::endl;
            return false;
        }
        std::shared_ptr<RenderEngine::ShaderProgram> shader_program_colliders = Resources::ResourceManager::getShaderProgram("ColliderShaderProgram");
        if (shader_program_colliders == nullptr)
        {
            std::cerr << "Can't load shader program " << "ColliderShaderProgram" << std::endl;
            return false;
        }*/


       /* std::shared_ptr<RenderEngine::Sprite2D> tank_sprite = Resources::ResourceManager::getSprite("yellowTankAnimatedSprite");
        if (tank_sprite == nullptr)
        {
            std::cerr << "Can't find animatad sprite: " << "yellowTankAnimatedSprite" << std::endl;
            return false;
        }*/

        //m_level = std::make_shared<Level>(std::move(*Resources::ResourceManager::getLevel(2)));

        //m_tank = std::make_shared<Tank>(tank_sprite, m_level->getPlayer1Respawn(), glm::vec2(Level::BLOCK_SIZE, Level::BLOCK_SIZE), 0.05f);
        //m_tank2 = std::make_shared<Tank>(std::move(tank_sprite), m_level->getPlayer2Respawn(), glm::vec2(Level::BLOCK_SIZE, Level::BLOCK_SIZE), 0.05f);

        //Physics::PhysicsEngine::addDynamicGameObject(m_tank);
        //Physics::PhysicsEngine::addDynamicGameObject(m_tank2);

        /*glm::mat4 projection_matrix = glm::ortho<float>(0.f, m_level->getLevelWidth(), 0.f, m_level->getLevelHeight(), -100.f, 100.f);
        shader_program_sprites->use();
        shader_program_sprites->setMatrix4("clip_matrix", projection_matrix);

        shader_program_colliders->use();
        shader_program_colliders->setMatrix4("clip_matrix", projection_matrix);*/

        m_current_game_state = Resources::ResourceManager::getGameState("Level6");
        if (m_current_game_state == nullptr)
        {
            std::cerr << "ERROR: Can't load game state" << std::endl;
            return false;
        }

        if (!m_current_game_state->start())
        {
            return false;
        }

        Physics::PhysicsEngine::setCurrentLevel(reinterpret_cast<std::shared_ptr<Level>&>(m_current_game_state));

        return true;
	}



	void Game::setKey(const int key, const int action)
	{
        m_keys[key] = action;
	}



	void Game::update(const double delta)
	{
       m_current_game_state->update(delta, m_keys);
	}



	void Game::render()
	{
        /*if (m_tank)
        {
            m_tank->render();
            m_tank->renderColliders();
        }
        if (m_tank2)
        {
            m_tank2->render();
            m_tank2->renderColliders();
        }
        if (m_level)
        {
            m_level->render();
        }*/
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