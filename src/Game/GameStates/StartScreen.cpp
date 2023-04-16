#include "StartScreen.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>

#include "../../Render/Sprite2D.h"

#include "../../Resources/ResourceManager.h"

#include <iostream>



static constexpr char* SHADER_PROGRAM_NAME = "spriteShaderProgram";
static constexpr char* SELECTOR_SPRITE_NAME = "whiteTank3_Right1";



static std::shared_ptr<RenderEngine::Sprite2D> createStartScreenElementFromDescription(char description)
{
	switch (description)
	{
	case '0':
		return Resources::ResourceManager::getSprite("BrickWall_8x8");
		break;
	case '1':
		return Resources::ResourceManager::getSprite("BrickWall_Left_Top_8x8");
		break;
	case '2':
		return Resources::ResourceManager::getSprite("BrickWall_Right_Top_8x8");
		break;
	case '3':
		return Resources::ResourceManager::getSprite("BrickWall_Top_8x8");
		break;
	case '4':
		return Resources::ResourceManager::getSprite("BrickWall_Left_Bottom_8x8");
		break;
	case '5':
		return Resources::ResourceManager::getSprite("BrickWall_Left_8x8");
		break;
	case '6':
		return Resources::ResourceManager::getSprite("BrickWall_Right_Top_Left_Bottom_8x8");
		break;
	case '7':
		return Resources::ResourceManager::getSprite("BrickWall_Without_Right_Bottom_8x8");
		break;
	case '8':
		return Resources::ResourceManager::getSprite("BrickWall_Right_Bottom_8x8");
		break;
	case '9':
		return Resources::ResourceManager::getSprite("BrickWall_Left_Top_Right_Bottom_8x8");
		break;
	case 'A':
		return Resources::ResourceManager::getSprite("BrickWall_Right_8x8");
		break;
	case 'B':
		return Resources::ResourceManager::getSprite("BrickWall_Without_Left_Bottom_8x8");
		break;
	case 'C':
		return Resources::ResourceManager::getSprite("BrickWall_Bottom_8x8");
		break;
	case 'D':
		return Resources::ResourceManager::getSprite("BrickWall_Without_Right_Top_8x8");
		break;
	case 'E':
		return Resources::ResourceManager::getSprite("BrickWall_Without_Left_Top_8x8");
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



namespace BatleCity
{
	std::shared_ptr<RenderEngine::ShaderProgram> StartScreen::m_start_srcreen_elements_shader_program = nullptr;



	StartScreen::StartScreen(const std::vector<std::string>& start_screen_description, uint16_t left_offset, uint16_t bottom_offset, 
							 uint16_t menu_position_x, uint16_t menu_position_y)
			: IGameState(EGameStates::StartScreen)
	{
		if (!m_start_srcreen_elements_shader_program)
		{
			setShaderProgram(Resources::ResourceManager::getShaderProgram(SHADER_PROGRAM_NAME));
		}

		if (start_screen_description.empty())
		{
			std::cerr << " => ERROR: Start screen description is empty" << std::endl;
		}
		else
		{
			m_width_blocks = start_screen_description[0].length();
			m_height_blocks = start_screen_description.size();
			m_width_pixels = m_width_blocks * BLOCK_SIZE + left_offset * 2;
			m_height_pixels = m_height_blocks * BLOCK_SIZE + bottom_offset * 2;

			m_start_screen_elements.reserve(static_cast<size_t>(m_width_blocks * m_height_blocks));

			unsigned int current_offset_y = (m_height_blocks - 1) * BLOCK_SIZE + bottom_offset;
			for (const std::string& current_row : start_screen_description)
			{
				unsigned int current_offset_x = left_offset;
				for (const char current_row_element : current_row)
				{
					m_start_screen_elements.emplace_back(std::make_pair<std::shared_ptr<RenderEngine::Sprite2D>, glm::vec2>(
																													createStartScreenElementFromDescription(current_row_element),
																													glm::vec2(current_offset_x, current_offset_y))
																															);
					current_offset_x += BLOCK_SIZE;
				}
				current_offset_y -= BLOCK_SIZE;
			}

			// menu selections
			m_menu_selections.emplace_back(std::make_pair<std::shared_ptr<RenderEngine::Sprite2D>, glm::vec2>(
										   Resources::ResourceManager::getSprite("One_Player_Selection"), glm::vec2(menu_position_x, menu_position_y)));

			m_menu_selections.emplace_back(std::make_pair<std::shared_ptr<RenderEngine::Sprite2D>, glm::vec2>(
										   Resources::ResourceManager::getSprite("Two_Players_Selection"), glm::vec2(menu_position_x, menu_position_y - BLOCK_SIZE * 2)));

			m_menu_selections.emplace_back(std::make_pair<std::shared_ptr<RenderEngine::Sprite2D>, glm::vec2>(
										   Resources::ResourceManager::getSprite("Construction_Selection"), glm::vec2(menu_position_x, menu_position_y - BLOCK_SIZE * 4)));
		
			
			// menu selector
			m_menu_selector = std::make_pair<std::shared_ptr<RenderEngine::Sprite2D>, glm::vec2>(Resources::ResourceManager::getSprite(SELECTOR_SPRITE_NAME),
																								 glm::vec2(menu_position_x - BLOCK_SIZE * 3, menu_position_y));
		}
	}



	void StartScreen::setShaderProgram(std::shared_ptr<RenderEngine::ShaderProgram>& shader_program) noexcept
	{
		m_start_srcreen_elements_shader_program = std::move(shader_program);
	}



	bool StartScreen::setProjectiomMatrix() const noexcept
	{
		glm::mat4 projection_matrix = glm::ortho<float>(0.f, getGameStateWidth(), 0.f, getGameStateHeight(), -100.f, 100.f);

		if (m_start_srcreen_elements_shader_program)
		{
			m_start_srcreen_elements_shader_program->use();
			m_start_srcreen_elements_shader_program->setMatrix4("clip_matrix", projection_matrix);
		}
		else
		{
			return false;
		}
		return true;
	}



	bool StartScreen::start() const noexcept
	{
		return setProjectiomMatrix();
	}



	size_t StartScreen::getGameStateWidth() const noexcept
	{
		return m_width_pixels;
	}



	size_t StartScreen::getGameStateHeight() const noexcept
	{
		return m_height_pixels;
	}



	StartScreen::EMenuPuncts StartScreen::select() const noexcept
	{
		if (m_menu_selector.second.y == m_menu_selections[0].second.y)
		{
			return EMenuPuncts::LevelOnePlayer;
		}
		else if (m_menu_selector.second.y == m_menu_selections[1].second.y)
		{
			return EMenuPuncts::LevelTwoPlayers;
		}
		else
		{
			return EMenuPuncts::Constructor;
		}
	}



	void StartScreen::update(double delta, std::array<bool, 349>& keyboard)
	{ 
		if (keyboard[GLFW_KEY_S])
		{
			bool is_above_than_bottom_selection = m_menu_selector.second.y > m_menu_selections[m_menu_selections.size() - 1].second.y;
			m_menu_selector.second.y = is_above_than_bottom_selection ? m_menu_selector.second.y - BLOCK_SIZE * 2 : m_menu_selections[0].second.y;
			keyboard[GLFW_KEY_S] = false;
		}
		else if (keyboard[GLFW_KEY_W])
		{
			bool is_less_than_upper_selection = m_menu_selector.second.y < m_menu_selections[0].second.y;
			m_menu_selector.second.y = is_less_than_upper_selection ? m_menu_selector.second.y + BLOCK_SIZE * 2 : m_menu_selections[m_menu_selections.size() - 1].second.y;
			keyboard[GLFW_KEY_W] = false;
		}
	}



	void StartScreen::render() const 
	{
		for (const auto& current_start_screen_element : m_start_screen_elements)
		{
			if (current_start_screen_element.first)
			{
				current_start_screen_element.first->render(current_start_screen_element.second, glm::vec2(BLOCK_SIZE), 0.f, 0.f);
			}
		}

		for (const auto& current_menu_selection : m_menu_selections)
		{
			if (current_menu_selection.first)
			{
				current_menu_selection.first->render(current_menu_selection.second, glm::vec2(MENU_SELECTION_WIDTH, MENU_SELECTION_HEIGHT), 0.f, 0.f);
			}
		}

		if (m_menu_selector.first)
		{
			m_menu_selector.first->render(m_menu_selector.second, glm::vec2(BLOCK_SIZE * 2), 0.f, 0.f);
		}
	}
}