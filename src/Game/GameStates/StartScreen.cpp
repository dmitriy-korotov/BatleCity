#include "StartScreen.h"

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>

#include "../GameObjects/BrickWall.h"

namespace BatleCity
{
	std::shared_ptr<IGameObject> createStartScreenElementFromDescription(char description, const glm::vec2& position,
																		 const glm::vec2& size, float rotation)
	{
		switch (description)
		{
		case '0':
			return std::make_shared<BrickWall>(Block::EBlockType::All, position, size, rotation, 0.f);
			break;
		case '1':
			return std::make_shared<BrickWall>(Block::EBlockType::LeftTop, position, size, rotation, 0.f);
			break;
		case '2':
			return std::make_shared<BrickWall>(Block::EBlockType::RightTop, position, size, rotation, 0.f);
			break;
		case '3':
			return std::make_shared<BrickWall>(Block::EBlockType::Top, position, size, rotation, 0.f);
			break;
		case '4':
			return std::make_shared<BrickWall>(Block::EBlockType::LeftBottom, position, size, rotation, 0.f);
			break;
		case '5':
			return std::make_shared<BrickWall>(Block::EBlockType::Left, position, size, rotation, 0.f);
			break;
		case '6':
			return std::make_shared<BrickWall>(Block::EBlockType::RightTop_LeftBottom, position, size, rotation, 0.f);
			break;
		case '7':
			return std::make_shared<BrickWall>(Block::EBlockType::WithoutRightBottom, position, size, rotation, 0.f);
			break;
		case '8':
			return std::make_shared<BrickWall>(Block::EBlockType::RightBottom, position, size, rotation, 0.f);
			break;
		case '9':
			return std::make_shared<BrickWall>(Block::EBlockType::LeftTop_RightBottom, position, size, rotation, 0.f);
			break;
		case 'A':
			return std::make_shared<BrickWall>(Block::EBlockType::Right, position, size, rotation, 0.f);
			break;
		case 'B':
			return std::make_shared<BrickWall>(Block::EBlockType::WithoutLeftBottom, position, size, rotation, 0.f);
			break;
		case 'C':
			return std::make_shared<BrickWall>(Block::EBlockType::Bottom, position, size, rotation, 0.f);
			break;
		case 'D':
			return std::make_shared<BrickWall>(Block::EBlockType::WithoutRightTop, position, size, rotation, 0.f);
			break;
		case 'E':
			return std::make_shared<BrickWall>(Block::EBlockType::WithoutLeftTop, position, size, rotation, 0.f);
			break;
		case 'F':
			return nullptr;
			break;
		default:
			std::cerr << "ERROR: Start srceen element with such description not found: " << description << std::endl;
			return nullptr;
			break;
		}
	}



	StartScreen::StartScreen(const std::vector<std::string>& start_screen_description)
		: IGameState(EGameStates::StartScreen)
	{
		if (start_screen_description.empty())
		{
			std::cerr << "Start screen description is empty" << std::endl;
		}
		else
		{
			m_width_blocks = start_screen_description[0].length();
			m_height_blocks = start_screen_description.size();
			m_width_pixels = m_width_blocks * BLOCK_SIZE;
			m_height_pixels = m_height_blocks * BLOCK_SIZE;

			m_start_screen_elements.reserve(static_cast<size_t>(m_width_blocks * m_height_blocks));

			unsigned int current_offset_y = (m_height_blocks - 1) * BLOCK_SIZE;
			for (const std::string& current_row : start_screen_description)
			{
				unsigned int current_offset_x = 0;
				for (const char current_row_element : current_row)
				{
					m_start_screen_elements.emplace_back(std::move(createStartScreenElementFromDescription(current_row_element,
																										   glm::vec2(current_offset_x, current_offset_y),
																										   glm::vec2(BLOCK_SIZE, BLOCK_SIZE),
																										   0.f)));
					current_offset_x += BLOCK_SIZE;
				}
				current_offset_y -= BLOCK_SIZE;
			}
		}
	}



	bool StartScreen::start() const noexcept
	{
		std::shared_ptr<RenderEngine::ShaderProgram> shader_program_sprites = Resources::ResourceManager::getShaderProgram("spriteShaderProgram");
		if (shader_program_sprites == nullptr)
		{
			std::cerr << "=> ERROR: Can't start level: => Can't load shader program: " << "spriteShaderProgram" << std::endl;
			return false;
		}

		std::shared_ptr<RenderEngine::ShaderProgram> shader_program_colliders = Resources::ResourceManager::getShaderProgram("ColliderShaderProgram");
		if (shader_program_colliders == nullptr)
		{
			std::cerr << "=> ERROR: Can't start level: => Can't load shader program: " << "ColliderShaderProgram" << std::endl;
			return false;
		}

		glm::mat4 projection_matrix = glm::ortho<float>(0.f, getGameStateWidth(), 0.f, getGameStateHeight(), -100.f, 100.f);
		shader_program_sprites->use();
		shader_program_sprites->setMatrix4("clip_matrix", projection_matrix);

		shader_program_colliders->use();
		shader_program_colliders->setMatrix4("clip_matrix", projection_matrix);

		return true;
	}



	size_t StartScreen::getGameStateWidth() const noexcept
	{
		return m_width_pixels;
	}



	size_t StartScreen::getGameStateHeight() const noexcept
	{
		return m_height_pixels;
	}



	void StartScreen::update(double delta, const std::array<bool, 349>&keyboard)
	{
		for (const auto& current_start_screen_element : m_start_screen_elements)
		{
			if (current_start_screen_element)
			{
				current_start_screen_element->update(delta);
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
	}



	void StartScreen::render() const 
	{
		for (const auto& current_start_screen_element : m_start_screen_elements)
		{
			if (current_start_screen_element)
			{
				current_start_screen_element->render();
				current_start_screen_element->renderColliders();
			}
		}
	}
}