#include "Level.h"

#include <iostream>
#include <algorithm>

#include "GameObjects/Block.h"
#include "GameObjects/BrickWall.h"
#include "GameObjects/BetonWAll.h"
#include "GameObjects/Tree.h"
#include "GameObjects/Ice.h"
#include "GameObjects/Water.h"
#include "GameObjects/Eagle.h"
#include "GameObjects/Border.h"

#include "../Resources/ResourceManager.h"



std::shared_ptr<BatleCity::IGameObject> createGameObjectFromDescription(const char description, const glm::vec2& position,
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
	Level::Level(const std::vector<std::string>& level_description)
	{
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

			m_map_objects.reserve(static_cast<size_t>(m_width_blocks * m_height_blocks + 4));


			// set default respawns
			m_player1_respawn = { (m_width_blocks / 2 - 1) * BLOCK_SIZE,	BLOCK_SIZE / 2.f };
			m_player2_respawn = { (m_width_blocks / 2 + 3) * BLOCK_SIZE,	BLOCK_SIZE / 2.f };
			m_enemy1_respawn  = { BLOCK_SIZE,								m_height_pixels - BLOCK_SIZE / 2.f };
			m_enemy2_respawn  = { (m_width_blocks / 2) * BLOCK_SIZE,		m_height_pixels - BLOCK_SIZE / 2.f };
			m_enemy3_respawn  = { m_width_pixels,							m_height_pixels - BLOCK_SIZE / 2.f };



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
						break;
					}
					m_map_objects.emplace_back(std::move(createGameObjectFromDescription(current_row_element,
																						 glm::vec2(current_offset_x, current_offset_y),
																						 glm::vec2(BLOCK_SIZE, BLOCK_SIZE),
																						 0.f)));
					current_offset_x += BLOCK_SIZE;
				}
				current_offset_y -= BLOCK_SIZE;
			}

			// border bottom
			m_map_objects.emplace_back(std::make_shared<Border>(glm::vec2(BLOCK_SIZE, 0.f), glm::vec2(m_width_pixels, BLOCK_SIZE / 2.f), 0.f, 0.f));

			// border top
			m_map_objects.emplace_back(std::make_shared<Border>(glm::vec2(BLOCK_SIZE, m_height_pixels + BLOCK_SIZE / 2.f),
																glm::vec2(m_width_pixels, BLOCK_SIZE / 2.f), 0.f, 0.f));

			// border left
			m_map_objects.emplace_back(std::make_shared<Border>(glm::vec2(0.f, 0.f), glm::vec2(BLOCK_SIZE, BLOCK_SIZE * (m_height_blocks + 1)), 0.f, 0.f));

			// border right
			m_map_objects.emplace_back(std::make_shared<Border>(glm::vec2(BLOCK_SIZE * (m_width_blocks + 1), 0.f),
																glm::vec2(BLOCK_SIZE * 2.f, BLOCK_SIZE * (m_height_blocks + 1)), 0.f, 0.f));
		}
	}



	Level::Level(Level&& other) noexcept
		: m_height_blocks(other.m_height_blocks)
		, m_width_blocks(other.m_width_blocks)
		, m_height_pixels(other.m_height_pixels)
		, m_width_pixels(other.m_width_pixels)
		, m_map_objects(std::move(other.m_map_objects))
		, m_player1_respawn(other.m_player1_respawn)
		, m_player2_respawn(other.m_player2_respawn)
		, m_enemy1_respawn(other.m_enemy1_respawn)
		, m_enemy2_respawn(other.m_enemy2_respawn)
		, m_enemy3_respawn(other.m_enemy3_respawn)
	{
		other.m_height_blocks = other.m_width_blocks = 0;
		other.m_player1_respawn = glm::vec2(0.f);
		other.m_player2_respawn = glm::vec2(0.f);
		other.m_enemy1_respawn = glm::vec2(0.f);
		other.m_enemy2_respawn = glm::vec2(0.f);
		other.m_enemy3_respawn = glm::vec2(0.f);
	}



	Level& Level::operator=(Level&& _right) noexcept
	{
		if (this != &_right)
		{
			m_map_objects = std::move(_right.m_map_objects);
			m_width_blocks = _right.m_width_blocks;
			m_height_blocks = _right.m_height_blocks;
			m_player1_respawn = _right.m_player1_respawn;
			m_player2_respawn = _right.m_player2_respawn;
			m_enemy1_respawn = _right.m_enemy1_respawn;
			m_enemy2_respawn = _right.m_enemy2_respawn;
			m_enemy3_respawn = _right.m_enemy3_respawn;
			_right.m_height_blocks = _right.m_width_blocks = 0;
			_right.m_player1_respawn = glm::vec2(0.f);
			_right.m_player2_respawn = glm::vec2(0.f);
			_right.m_enemy1_respawn = glm::vec2(0.f);
			_right.m_enemy2_respawn = glm::vec2(0.f);
			_right.m_enemy3_respawn = glm::vec2(0.f);
		}
		return *this;
	}



	size_t Level::getLevelWidth() const
	{
		return static_cast<size_t>(BLOCK_SIZE * (m_width_blocks + 3));
	}



	size_t Level::getLevelHeight() const
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
				const auto& object = m_map_objects[y_index * m_width_blocks + x_index];
				if (object)
				{
					objects_in_area.emplace_back(object);
				}
			}
		}

		if (end_X == 13)
		{
			objects_in_area.emplace_back(m_map_objects[m_map_objects.size() - 1]);
		}
		if (start_Y == 0)
		{
			objects_in_area.emplace_back(m_map_objects[m_map_objects.size() - 3]);
		}
		if (start_X == 0)
		{
			objects_in_area.emplace_back(m_map_objects[m_map_objects.size() - 2]);
		}
		if (end_Y == 14)
		{
			objects_in_area.emplace_back(m_map_objects[m_map_objects.size() - 4]);
		}

		//std::cout << "Amount objects in area: " << objects_in_area.size() << std::endl;
		//std::cout << "Start X: " << start_X << "\t\t" << "End X : " << end_X << std::endl;
		//std::cout << "Start Y: " << start_Y << "\t\t" << "End Y : " << end_Y << std::endl;
		//std::cout << "Position X: " << position.x << "\t\t" << "Position Y : " << position.y << std::endl;
		//std::cout << "Convent Position X: " << convent_position.x << "\t\t" << "Convent Position Y: " << convent_position.y << std::endl;

		return objects_in_area;
	}



	void Level::update(const double delta)
	{
		for (const auto& current_map_object : m_map_objects)
		{
			if (current_map_object)
			{
				current_map_object->update(delta);
			}
		}
	}



	void Level::render() const
	{
		for (const auto& current_map_object : m_map_objects)
		{
			if (current_map_object)
			{
				current_map_object->render();
				for (const auto& collider : current_map_object->getColliders())
				{
					collider.render(current_map_object->getPosition(), current_map_object->getRotation());
				}
			}
		}
	}
}