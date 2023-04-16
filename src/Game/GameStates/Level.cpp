#include "Level.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>

#include "../GameObjects/BrickWall.h"
#include "../GameObjects/BetonWAll.h"
#include "../GameObjects/Tree.h"
#include "../GameObjects/Ice.h"
#include "../GameObjects/Water.h"
#include "../GameObjects/Eagle.h"
#include "../GameObjects/Border.h"
#include "../GameObjects/Tank.h"

#include "../../Physics/PhysicsEngine.h"

#include "../../Resources/ResourceManager.h"

#include <iostream>
#include <algorithm>



static constexpr char* GAME_OBJECTS_SHADER_PROGRAM_NAME = "spriteShaderProgram";
static constexpr char* COLLIDERS_SHADER_PROGRAM_NAME = "colliderShaderProgram";



static std::shared_ptr<BatleCity::IGameObject> createGameObjectFromDescription(const char description, const glm::vec2& position,
																			   const glm::vec2& size, const float rotation)
{
	switch (description)
	{
	case '0':
		return std::make_shared<BatleCity::BrickWall>(BatleCity::Block::EBlockType::Right, position, size, rotation, 0.f);
		break;
	case '1':
		return std::make_shared<BatleCity::BrickWall>(BatleCity::Block::EBlockType::Bottom, position, size, rotation, 0.f);
		break;
	case '2':
		return std::make_shared<BatleCity::BrickWall>(BatleCity::Block::EBlockType::Left, position, size, rotation, 0.f);
		break;
	case '3':
		return std::make_shared<BatleCity::BrickWall>(BatleCity::Block::EBlockType::Top, position, size, rotation, 0.f);
		break;
	case '4':
		return std::make_shared<BatleCity::BrickWall>(BatleCity::Block::EBlockType::All, position, size, rotation, 0.f);
		break;
	case 'H':
		return std::make_shared<BatleCity::BrickWall>(BatleCity::Block::EBlockType::RightBottom, position, size, rotation, 0.f);
		break;
	case 'G':
		return std::make_shared<BatleCity::BrickWall>(BatleCity::Block::EBlockType::LeftBottom, position, size, rotation, 0.f);
		break;
	case '5':
		return std::make_shared<BatleCity::BetonWall>(BatleCity::Block::EBlockType::Right, position, size, rotation, 0.f);
		break;
	case '6':
		return std::make_shared<BatleCity::BetonWall>(BatleCity::Block::EBlockType::Bottom, position, size, rotation, 0.f);
		break;
	case '7':
		return std::make_shared<BatleCity::BetonWall>(BatleCity::Block::EBlockType::Left, position, size, rotation, 0.f);
		break;
	case '8':
		return std::make_shared<BatleCity::BetonWall>(BatleCity::Block::EBlockType::Top, position, size, rotation, 0.f);
		break;
	case '9':
		return std::make_shared<BatleCity::BetonWall>(BatleCity::Block::EBlockType::All, position, size, rotation, 0.f);
		break;
	case 'A':
		return std::make_shared<BatleCity::Water>(BatleCity::Block::EBlockType::All, position, size, rotation, -1.f);
		break;
	case 'B':
		return std::make_shared<BatleCity::Tree>(BatleCity::Block::EBlockType::All, position, size, rotation, 1.f);
		break;
	case 'C':
		return std::make_shared<BatleCity::Ice>(BatleCity::Block::EBlockType::All, position, size, rotation, -1.f);
		break;
	case 'D':
		return nullptr;
		break;
	case 'E':
		return std::make_shared<BatleCity::Eagle>(position, size, rotation, 0.f);
		break;
	default:
		std::cerr << "ERROR: Game object with such description not found: " << description << std::endl;
		return nullptr;
	}
}



namespace BatleCity
{
	std::shared_ptr<RenderEngine::ShaderProgram> Level::m_game_obgects_shader_program = nullptr;
	std::shared_ptr<RenderEngine::ShaderProgram> Level::m_colliders_shader_program = nullptr;



	Level::Level(const std::vector<std::string>& level_description)
		: IGameState(EGameStates::Level)
	{
		if (!m_game_obgects_shader_program)
		{
			setGameObjectsShaderProgram(Resources::ResourceManager::getShaderProgram(GAME_OBJECTS_SHADER_PROGRAM_NAME));
		}
		if (!m_colliders_shader_program)
		{
			setCollidersShaderProgram(Resources::ResourceManager::getShaderProgram(COLLIDERS_SHADER_PROGRAM_NAME));
		}

		if (level_description.empty())
		{
			std::cerr << "Level description is empty" << std::endl;
		}
		else
		{
			m_width_blocks = level_description[0].length();
			m_height_blocks = level_description.size();
			m_width_pixels = m_width_blocks * BLOCK_SIZE;
			m_height_pixels = m_height_blocks * BLOCK_SIZE;

			m_player1_respawn = { (m_width_blocks / 2 - 1) * BLOCK_SIZE,	BLOCK_SIZE / 2.f };
			m_player2_respawn = { (m_width_blocks / 2 + 3) * BLOCK_SIZE,	BLOCK_SIZE / 2.f };
			m_enemy1_respawn =  { BLOCK_SIZE,								m_height_pixels - BLOCK_SIZE / 2.f };
			m_enemy2_respawn =  { (m_width_blocks / 2) * BLOCK_SIZE,		m_height_pixels - BLOCK_SIZE / 2.f };
			m_enemy3_respawn =  { m_width_pixels,							m_height_pixels - BLOCK_SIZE / 2.f };

			m_static_map_objects.reserve(static_cast<size_t>(m_width_blocks * m_height_blocks + 4));

			unsigned int current_offset_y = (m_height_blocks - 0.5f) * BLOCK_SIZE;
			for (const std::string& current_row : level_description)
			{
				unsigned int current_offset_x = BLOCK_SIZE;
				for (const char current_row_element : current_row)
				{
					switch (current_row_element)
					{
					case 'K':
						m_player1_respawn = { current_offset_x, current_offset_y };
						break;
					case 'L':
						m_player2_respawn = { current_offset_x, current_offset_y };
						break;
					case 'M':
						m_enemy1_respawn = { current_offset_x, current_offset_y };
						break;
					case 'N':
						m_enemy2_respawn = { current_offset_x, current_offset_y };
						break;
					case 'O':
						m_enemy3_respawn = { current_offset_x, current_offset_y };
						break;
					default:
						m_static_map_objects.emplace_back(std::move(createGameObjectFromDescription(current_row_element,
														  glm::vec2(current_offset_x, current_offset_y),
														  glm::vec2(BLOCK_SIZE, BLOCK_SIZE),
														  0.f)));
						break;
					}
					current_offset_x += BLOCK_SIZE;
				}
				current_offset_y -= BLOCK_SIZE;
			}

			// border bottom
			m_static_map_objects.emplace_back(std::make_shared<Border>(glm::vec2(BLOCK_SIZE, 0.f), glm::vec2(m_width_pixels, BLOCK_SIZE / 2.f), 0.f, 0.f));

			// border top
			m_static_map_objects.emplace_back(std::make_shared<Border>(glm::vec2(BLOCK_SIZE, m_height_pixels + BLOCK_SIZE / 2.f),
																	   glm::vec2(m_width_pixels, BLOCK_SIZE / 2.f), 0.f, 0.f));

			// border left
			m_static_map_objects.emplace_back(std::make_shared<Border>(glm::vec2(0.f, 0.f), glm::vec2(BLOCK_SIZE, BLOCK_SIZE * (m_height_blocks + 1)), 0.f, 0.f));

			// border right
			m_static_map_objects.emplace_back(std::make_shared<Border>(glm::vec2(BLOCK_SIZE * (m_width_blocks + 1), 0.f),
																	   glm::vec2(BLOCK_SIZE * 2.f, BLOCK_SIZE * (m_height_blocks + 1)), 0.f, 0.f));
		
			// add tanks
			m_dynamic_map_objects.emplace_back(std::make_shared<Tank>(Tank::ETankType::RedTank1,
											   m_player1_respawn, glm::vec2(BLOCK_SIZE, BLOCK_SIZE), 0.05f));

			m_dynamic_map_objects.emplace_back(std::make_shared<Tank>(Tank::ETankType::GreenTank5,
											   m_player2_respawn, glm::vec2(BLOCK_SIZE, BLOCK_SIZE), 0.05f));
		}
	}



	void Level::setGameObjectsShaderProgram(std::shared_ptr<RenderEngine::ShaderProgram>& shader_program) noexcept
	{
		m_game_obgects_shader_program = std::move(shader_program);
	}



	void Level::setCollidersShaderProgram(std::shared_ptr<RenderEngine::ShaderProgram>& shader_program) noexcept
	{
		m_colliders_shader_program = std::move(shader_program);
	}



	void Level::initPhysics() const
	{
		Physics::PhysicsEngine::addDynamicGameObject(m_dynamic_map_objects[0]);
		Physics::PhysicsEngine::addDynamicGameObject(m_dynamic_map_objects[1]);

		try
		{
			std::shared_ptr<const Level> ptr = shared_from_this();
		}
		catch (...)
		{
			std::cerr << "ERROR: Exception" << std::endl;
		}
	}



	bool Level::setProjectiomMatrix() const noexcept
	{
		glm::mat4 projection_matrix = glm::ortho<float>(0.f, getGameStateWidth(), 0.f, getGameStateHeight(), -100.f, 100.f);

		if (m_game_obgects_shader_program)
		{
			m_game_obgects_shader_program->use();
			m_game_obgects_shader_program->setMatrix4("clip_matrix", projection_matrix);
		}
		else
		{
			return false;
		}

		if (m_colliders_shader_program)
		{
			m_colliders_shader_program->use();
			m_colliders_shader_program->setMatrix4("clip_matrix", projection_matrix);
		}
		else
		{
			return false;
		}

		return true;
	}



	bool Level::start() const noexcept
	{
		if (!setProjectiomMatrix())
		{
			return false;
		}
		initPhysics();
		return true;
	}



	size_t Level::getGameStateWidth() const noexcept
	{
		return static_cast<size_t>(BLOCK_SIZE * (m_width_blocks + 3));
	}



	size_t Level::getGameStateHeight() const noexcept
	{
		return static_cast<size_t>(BLOCK_SIZE * (m_height_blocks + 1));
	}



	std::vector<std::shared_ptr<BatleCity::IGameObject>> Level::getObjectsFromArea(const glm::vec2& position, const glm::vec2& size) const
	{
		std::vector<std::shared_ptr<BatleCity::IGameObject>> objects_in_area;

		glm::vec2 convent_position(std::clamp(position.x - BLOCK_SIZE, 0.f, static_cast<float>(m_width_pixels)),
								   std::clamp(m_height_pixels - position.y - BLOCK_SIZE / 2, 0.f, static_cast<float>(m_height_pixels)));

		unsigned int start_X = static_cast<unsigned int>(std::floor(std::clamp(convent_position.x, 0.f, static_cast<float>(m_width_pixels)) / BLOCK_SIZE));
		unsigned int end_X = static_cast<unsigned int>(std::ceil(std::clamp(convent_position.x + size.x, 0.f, static_cast<float>(m_width_pixels)) / BLOCK_SIZE));

		unsigned int start_Y = static_cast<unsigned int>(std::floor(std::clamp(convent_position.y, 0.f, static_cast<float>(m_height_pixels)) / BLOCK_SIZE));
		unsigned int end_Y = static_cast<unsigned int>(std::ceil((std::clamp(convent_position.y + size.y, 0.f, static_cast<float>(m_height_pixels)) / BLOCK_SIZE)));

		for (unsigned int x_index = start_X; x_index < end_X; ++x_index)
		{
			for (unsigned int y_index = start_Y; y_index < end_Y; ++y_index)
			{
				const auto& object = m_static_map_objects[static_cast<size_t>(y_index) * m_width_blocks + x_index];
				if (object)
				{
					objects_in_area.emplace_back(object);
				}
			}
		}

		if (end_X == 13)
		{
			objects_in_area.emplace_back(m_static_map_objects[m_static_map_objects.size() - 1]);
		}
		if (start_Y == 0)
		{
			objects_in_area.emplace_back(m_static_map_objects[m_static_map_objects.size() - 3]);
		}
		if (start_X == 0)
		{
			objects_in_area.emplace_back(m_static_map_objects[m_static_map_objects.size() - 2]);
		}
		if (end_Y == 14)
		{
			objects_in_area.emplace_back(m_static_map_objects[m_static_map_objects.size() - 4]);
		}
		return objects_in_area;
	}



	void Level::update(const double delta, std::array<bool, 349>& keyboard)
	{
		for (const auto& current_static_map_object : m_static_map_objects)
		{
			if (current_static_map_object)
			{
				current_static_map_object->update(delta);
			}
		}

		for (const auto& current_dynamic_map_object : m_dynamic_map_objects)
		{
			if (current_dynamic_map_object)
			{
				current_dynamic_map_object->update(delta);
			}
		}

		if (keyboard[GLFW_KEY_F])
		{
			IGameObject::enableRenderingColliders();
		}
		if (keyboard[GLFW_KEY_G])
		{
			IGameObject::disableRenderingColliders();
		}
		if (keyboard[GLFW_KEY_W])
		{
			m_dynamic_map_objects[0]->setOrientation(IDynamicGameObject::EOrientation::Top);
			if (keyboard[GLFW_KEY_LEFT_SHIFT])
			{
				m_dynamic_map_objects[0]->setVelocity(m_dynamic_map_objects[0]->getMaxVelocity() / 2);
			}
			else
			{
				m_dynamic_map_objects[0]->setVelocity(m_dynamic_map_objects[0]->getMaxVelocity());
			}
		}
		else if (keyboard[GLFW_KEY_D])
		{
			m_dynamic_map_objects[0]->setOrientation(IDynamicGameObject::EOrientation::Right);
			if (keyboard[GLFW_KEY_LEFT_SHIFT])
			{
				m_dynamic_map_objects[0]->setVelocity(m_dynamic_map_objects[0]->getMaxVelocity() / 2);
			}
			else
			{
				m_dynamic_map_objects[0]->setVelocity(m_dynamic_map_objects[0]->getMaxVelocity());
			}
		}
		else if (keyboard[GLFW_KEY_S])
		{
			m_dynamic_map_objects[0]->setOrientation(IDynamicGameObject::EOrientation::Bottom);
			if (keyboard[GLFW_KEY_LEFT_SHIFT])
			{
				m_dynamic_map_objects[0]->setVelocity(m_dynamic_map_objects[0]->getMaxVelocity() / 2);
			}
			else
			{
				m_dynamic_map_objects[0]->setVelocity(m_dynamic_map_objects[0]->getMaxVelocity());
			}
		}
		else if (keyboard[GLFW_KEY_A])
		{
			m_dynamic_map_objects[0]->setOrientation(IDynamicGameObject::EOrientation::Left);
			if (keyboard[GLFW_KEY_LEFT_SHIFT])
			{
				m_dynamic_map_objects[0]->setVelocity(m_dynamic_map_objects[0]->getMaxVelocity() / 2);
			}
			else
			{
				m_dynamic_map_objects[0]->setVelocity(m_dynamic_map_objects[0]->getMaxVelocity());
			}
		}
		else
		{
			m_dynamic_map_objects[0]->setVelocity(0);
		}

		if (keyboard[GLFW_KEY_UP])
		{
			m_dynamic_map_objects[1]->setOrientation(Tank::EOrientation::Top);
			m_dynamic_map_objects[1]->setVelocity(m_dynamic_map_objects[1]->getMaxVelocity());
		}
		else if (keyboard[GLFW_KEY_RIGHT])
		{
			m_dynamic_map_objects[1]->setOrientation(Tank::EOrientation::Right);
			m_dynamic_map_objects[1]->setVelocity(m_dynamic_map_objects[1]->getMaxVelocity());
		}
		else if (keyboard[GLFW_KEY_DOWN])
		{
			m_dynamic_map_objects[1]->setOrientation(Tank::EOrientation::Bottom);
			m_dynamic_map_objects[1]->setVelocity(m_dynamic_map_objects[1]->getMaxVelocity());
		}
		else if (keyboard[GLFW_KEY_LEFT])
		{
			m_dynamic_map_objects[1]->setOrientation(Tank::EOrientation::Left);
			m_dynamic_map_objects[1]->setVelocity(m_dynamic_map_objects[1]->getMaxVelocity());
		}
		else
		{
			m_dynamic_map_objects[1]->setVelocity(0);
		}

		static Tank* tank1_ptr = dynamic_cast<Tank*>(&(*m_dynamic_map_objects[0]));
		static Tank* tank2_ptr = dynamic_cast<Tank*>(&(*m_dynamic_map_objects[1]));

		if (tank1_ptr)
		{
			if (keyboard[GLFW_KEY_SPACE])
			{
				tank1_ptr->fair();
			}
		}

		if (tank2_ptr)
		{
			if (keyboard[GLFW_KEY_ENTER])
			{
				tank2_ptr->fair();
			}
		}

		Physics::PhysicsEngine::update(delta);
	}



	void Level::render() const
	{
		for (const auto& current_static_map_object : m_static_map_objects)
		{
			if (current_static_map_object)
			{
				current_static_map_object->render();
				current_static_map_object->renderColliders();
			}
		}
		for (const auto& current_dynamic_map_object : m_dynamic_map_objects)
		{
			if (current_dynamic_map_object)
			{
				current_dynamic_map_object->render();
				current_dynamic_map_object->renderColliders();
			}
		}
	}
}