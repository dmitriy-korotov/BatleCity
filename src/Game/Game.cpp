#include "Game.h"
#include "../Resources/ResourceManager.h"
#include "../Render/ShaderProgram.h"
#include "../Render/Texture2D.h"
#include "../Render/Sprite2D.h"
#include "../Render/AnimatedSprite2D.h"
#include "Tank.h"

#include <GLFW/glfw3.h>
#include <iostream>

namespace batle_city
{
	Game::Game(const glm::vec2& window_size) : m_window_size(window_size),
											   m_current_game_state(Game::EGameStates::Active)
	{ 
		m_keys.fill(false);
	}



	Game::~Game()
	{
        //Resources::ResourceManager::unloadAllResources();
	}



	bool Game::init()
	{
        std::shared_ptr<Renderer::ShaderProgram> shader_program = Resources::ResourceManager::loadShaderProrgam("Shader_program_1",
            "res/shaders/vertex.txt",
            "res/shaders/fragment.txt");
        if (!shader_program->isCompiled())
        {
            std::cerr << "Can't compiled shader program" << std::endl;
            return false;
        }

        std::shared_ptr<Renderer::ShaderProgram> shader_program_sprite = Resources::ResourceManager::loadShaderProrgam("Shader_program_2",
            "res/shaders/vSprite.txt",
            "res/shaders/fSprite.txt");
        if (!shader_program_sprite->isCompiled())
        {
            std::cerr << "Can't compiled shader program" << std::endl;
            return false;
        }

        std::vector<std::string> names_subTextures = { "block", "topBlock", "bottomBlock", "leftBlock", "rightBlock", "leftTopBlock",
                                                      "rightTopBlock", "leftBottomBlock", "rightBottomBlock", "beton", "topBeton",
                                                      "bottomBeton", "leftBeton", "rightBeton", "leftTopBeton", "rightTopBeton",
                                                      "leftBottomBeton", "rightBottomBeton", "water1", "water2", "water3", "trees",
                                                      "arma", "grey", "eagle", "eagleDead" };

        std::shared_ptr<Renderer::Texture2D> texture_2 = Resources::ResourceManager::loadTextureAtlas("Texture_2", names_subTextures,
            "res/textures/map_16x16.png", 16, 16);

        std::shared_ptr<Renderer::AnimatedSprite2D> sprite_1 = Resources::ResourceManager::loadAnimatedSprite("Sprite1", "Shader_program_2",
            "Texture_2", 500.f, 500.f, "beton");

        std::shared_ptr<Renderer::Sprite2D> sprite_2 = Resources::ResourceManager::loadSprite("Sprite2", "Shader_program_2",
            "Texture_2", 300.f, 300.f, "beton");

        std::shared_ptr<Renderer::Sprite2D> sprite_3 = Resources::ResourceManager::loadSprite("Sprite3", "Shader_program_2",
            "Texture_2", 300.f, 300.f, "rightBottomBeton");

        std::vector<std::pair<std::string, uint64_t>> water_state;

        water_state.push_back(std::make_pair<std::string, uint64_t>("water1", 300000000));
        water_state.push_back(std::make_pair<std::string, uint64_t>("water2", 300000000));
        water_state.push_back(std::make_pair<std::string, uint64_t>("water3", 300000000));

        std::vector<std::pair<std::string, uint64_t>> eagle_state;

        eagle_state.push_back(std::make_pair<std::string, uint64_t>("eagle", 300000000));
        eagle_state.push_back(std::make_pair<std::string, uint64_t>("eagleDead", 300000000));

        sprite_1->addState("Water", water_state);
        sprite_1->addState("Eagle", eagle_state);

        sprite_1->setState("Water");

        sprite_1->setPosition(glm::vec2(350.f, 150.f));
        sprite_3->setPosition(glm::vec2(0.f, 420.f));



        std::vector<std::string> name_tanks_subTextures = { "TankTop1", "TankTop2" , "TankLeft1" , "TankLeft2",
                                                           "TankBottom1" , "TankBottom2", "TankRight1", "TankRight2" };

        std::shared_ptr<Renderer::Texture2D> tank_texture_atlas = Resources::ResourceManager::loadTextureAtlas("TankTextureAtlas", name_tanks_subTextures,
                                                                                                               "res/textures/tanks.png", 16, 16);

        std::shared_ptr<Renderer::AnimatedSprite2D> tank_sprite = Resources::ResourceManager::loadAnimatedSprite("Tank1", "Shader_program_2",
            "TankTextureAtlas", 100.f, 100.f, "TankTop1");

        std::vector<std::pair<std::string, uint64_t>> tank_top;
        tank_top.push_back(std::make_pair<std::string, uint64_t>("TankTop1", 350000000));
        tank_top.push_back(std::make_pair<std::string, uint64_t>("TankTop2", 350000000));

        std::vector<std::pair<std::string, uint64_t>> tank_left;
        tank_left.push_back(std::make_pair<std::string, uint64_t>("TankLeft1", 350000000));
        tank_left.push_back(std::make_pair<std::string, uint64_t>("TankLeft2", 350000000));


        std::vector<std::pair<std::string, uint64_t>> tank_bottom;
        tank_bottom.push_back(std::make_pair<std::string, uint64_t>("TankBottom1", 350000000));
        tank_bottom.push_back(std::make_pair<std::string, uint64_t>("TankBottom2", 350000000));

        std::vector<std::pair<std::string, uint64_t>> tank_right;
        tank_right.push_back(std::make_pair<std::string, uint64_t>("TankRight1", 350000000));
        tank_right.push_back(std::make_pair<std::string, uint64_t>("TankRight2", 350000000));

        tank_sprite->addState("TankTopState", tank_top);
        tank_sprite->addState("TankRightState", tank_right);
        tank_sprite->addState("TankBottomState", tank_bottom);
        tank_sprite->addState("TankLeftState", tank_left);

        tank_sprite->setState("TankTopState");

        m_tank = std::make_unique<Tank>(tank_sprite, glm::vec2(0.f, 0.f), 0.0000001f);
		
        return true;
	}



	void Game::setKey(const int key, const int action)
	{
        m_keys[key] = action;
	}

	void Game::update(const uint64_t delta)
	{
       // Resources::ResourceManager::getAnimatedSprite("Sprite1")->update(delta);
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
	}



	void Game::render()
	{
        /*Resources::ResourceManager::getAnimatedSprite("Sprite1")->render();
        Resources::ResourceManager::getSprite("Sprite2")->render();
        Resources::ResourceManager::getSprite("Sprite3")->render();*/

        if (m_tank)
        {
            m_tank->render();
        }
	}
}