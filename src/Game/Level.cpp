#include "Level.h"

#include <iostream>

#include "GameObjects/Block.h"
#include "GameObjects/BrickWall.h"
#include "GameObjects/BetonWAll.h"
#include "GameObjects/Tree.h"
#include "GameObjects/Ice.h"
#include "GameObjects/Water.h"
#include "GameObjects/Eagle.h"
#include "GameObjects/Border.h"

#include "../Resources/ResourceManager.h"



const unsigned int BLOCK_SIZE = 16;



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
			m_width = level_description[0].length();
			m_height = level_description.size();

			m_map_objects.reserve(m_width * m_height + 4);

			unsigned int current_offset_y = (m_height - 0.5f) * BLOCK_SIZE;
			for (const std::string& current_row : level_description)
			{
				unsigned int current_offset_x = BLOCK_SIZE;
				for (const char current_row_element : current_row)
				{
					m_map_objects.emplace_back(std::move(createGameObjectFromDescription(current_row_element,
																						 glm::vec2(current_offset_x, current_offset_y),
																						 glm::vec2(BLOCK_SIZE, BLOCK_SIZE),
																						 0.f)));
					current_offset_x += BLOCK_SIZE;
				}
				current_offset_y -= BLOCK_SIZE;
			}

			// border bottom
			m_map_objects.emplace_back(std::make_shared<Border>(glm::vec2(BLOCK_SIZE, 0.f), glm::vec2(m_width * BLOCK_SIZE, BLOCK_SIZE / 2.f), 0.f, 0.f));

			// border top
			m_map_objects.emplace_back(std::make_shared<Border>(glm::vec2(BLOCK_SIZE, m_height * BLOCK_SIZE + BLOCK_SIZE / 2.f),
																glm::vec2(m_width * BLOCK_SIZE, BLOCK_SIZE / 2.f), 0.f, 0.f));

			// border left
			m_map_objects.emplace_back(std::make_shared<Border>(glm::vec2(0.f, 0.f), glm::vec2(BLOCK_SIZE, BLOCK_SIZE * (m_height + 1)), 0.f, 0.f));

			// border right
			m_map_objects.emplace_back(std::make_shared<Border>(glm::vec2(BLOCK_SIZE * (m_width + 1), 0.f),
																glm::vec2(BLOCK_SIZE * 2.f, BLOCK_SIZE * (m_height + 1)), 0.f, 0.f));
		}
	}



	Level::Level(Level&& other) noexcept : m_height(other.m_height), m_width(other.m_width), m_map_objects(std::move(other.m_map_objects))
	{
		other.m_height = other.m_width = 0;
	}



	Level& Level::operator=(Level&& _right) noexcept
	{
		if (this != &_right)
		{
			m_map_objects = std::move(_right.m_map_objects);
			m_width = _right.m_width;
			m_height = _right.m_height;
			_right.m_height = _right.m_width = 0;
		}
		return *this;
	}



	size_t Level::getLevelWidth() const
	{
		return BLOCK_SIZE * (m_width + 3);
	}



	size_t Level::getLevelHeight() const
	{
		return BLOCK_SIZE * (m_height + 1);
	}



	void Level::update(const uint64_t delta)
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
			}
		}
	}
}