#pragma once
#ifndef START_SCREEN_H
#define START_SCREEN_H

#include <glm/vec2.hpp>

#include "IGameState.h"

#include <string>
#include <vector>
#include <memory>



namespace RenderEngine
{
	class Sprite2D;
}


namespace BatleCity
{
	class StartScreen : public IGameState
	{
	public:

		static constexpr unsigned int BLOCK_SIZE = 8;
		static constexpr unsigned int MENU_SELECTION_WIDTH = 128;
		static constexpr unsigned int MENU_SELECTION_HEIGHT = 16;



		enum class EMenuPuncts : uint8_t
		{
			LevelOnePlayer,
			LevelTwoPlayers,
			Constructor
		};

		StartScreen(const std::vector<std::string>& start_screen_description, uint16_t left_offset, uint16_t bottom_offset,
					uint16_t menu_position_x, uint16_t menu_position_y);
		~StartScreen() { };

		size_t getGameStateWidth() const noexcept override;
		size_t getGameStateHeight() const noexcept override;

		bool start() const noexcept override;
		void render() const override;
		void update(double delta, std::array<bool, 349>& keyboard) override;

		EMenuPuncts select() const noexcept;

	private:

		static void setShaderProgram(std::shared_ptr<RenderEngine::ShaderProgram>& shader_program) noexcept;

		bool setProjectiomMatrix() const noexcept;



		static std::shared_ptr<RenderEngine::ShaderProgram> m_start_srcreen_elements_shader_program;

		std::vector<std::pair<std::shared_ptr<RenderEngine::Sprite2D>, glm::vec2>> m_start_screen_elements;

		std::vector<std::pair<std::shared_ptr<RenderEngine::Sprite2D>, glm::vec2>> m_menu_selections;
		std::pair<std::shared_ptr<RenderEngine::Sprite2D>, glm::vec2> m_menu_selector;
		
	};
}

#endif // !START_SCREEN_H
