#include "StartScreen.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>

#include <BatleCity/Game/Game.h>

#include "../../Render/Sprite2D.h"

#include "../../Resources/ResourceManager.h"

#include <iostream>

static constexpr const char* SHADER_PROGRAM_NAME = "spriteShaderProgram";
static constexpr const char* SELECTOR_SPRITE_NAME = "whiteTank3_Right1";

static std::shared_ptr<RenderEngine::Sprite2D>
createStartScreenElementFromDescription(char description) {
  switch (description) {
    case '0':
      return BatleCity::Game::Instance().GetResourcesManager()->GetSprite(
          "BrickWall_8x8");
    case '1':
      return BatleCity::Game::Instance().GetResourcesManager()->GetSprite(
          "BrickWall_Left_Top_8x8");
    case '2':
      return BatleCity::Game::Instance().GetResourcesManager()->GetSprite(
          "BrickWall_Right_Top_8x8");
    case '3':
      return BatleCity::Game::Instance().GetResourcesManager()->GetSprite(
          "BrickWall_Top_8x8");
    case '4':
      return BatleCity::Game::Instance().GetResourcesManager()->GetSprite(
          "BrickWall_Left_Bottom_8x8");
    case '5':
      return BatleCity::Game::Instance().GetResourcesManager()->GetSprite(
          "BrickWall_Left_8x8");
    case '6':
      return BatleCity::Game::Instance().GetResourcesManager()->GetSprite(
          "BrickWall_Right_Top_Left_Bottom_8x8");
    case '7':
      return BatleCity::Game::Instance().GetResourcesManager()->GetSprite(
          "BrickWall_Without_Right_Bottom_8x8");
    case '8':
      return BatleCity::Game::Instance().GetResourcesManager()->GetSprite(
          "BrickWall_Right_Bottom_8x8");
    case '9':
      return BatleCity::Game::Instance().GetResourcesManager()->GetSprite(
          "BrickWall_Left_Top_Right_Bottom_8x8");
    case 'A':
      return BatleCity::Game::Instance().GetResourcesManager()->GetSprite(
          "BrickWall_Right_8x8");
    case 'B':
      return BatleCity::Game::Instance().GetResourcesManager()->GetSprite(
          "BrickWall_Without_Left_Bottom_8x8");
    case 'C':
      return BatleCity::Game::Instance().GetResourcesManager()->GetSprite(
          "BrickWall_Bottom_8x8");
    case 'D':
      return BatleCity::Game::Instance().GetResourcesManager()->GetSprite(
          "BrickWall_Without_Right_Top_8x8");
    case 'E':
      return BatleCity::Game::Instance().GetResourcesManager()->GetSprite(
          "BrickWall_Without_Left_Top_8x8");
    case 'F':
      return nullptr;
    default:
      std::cerr
          << "WARNING: Start srceen element with such description not found: "
          << description << std::endl;
      return nullptr;
  }
}

namespace BatleCity {
std::shared_ptr<RenderEngine::ShaderProgram>
    StartScreen::m_start_srcreen_elements_shader_program = nullptr;

StartScreen::StartScreen(
    const std::vector<std::string>& start_screen_description,
    uint16_t left_offset, uint16_t bottom_offset, uint16_t menu_position_x,
    uint16_t menu_position_y)
    : IGameState(EGameStates::StartScreen) {
  if (!m_start_srcreen_elements_shader_program) {
    setShaderProgram(Game::Instance().GetResourcesManager()->GetShaderProgram(
        SHADER_PROGRAM_NAME));
  }

  if (start_screen_description.empty()) {
    std::cerr << " => ERROR: Start screen description is empty" << std::endl;
  } else {
    m_width_blocks = start_screen_description[0].length();
    m_height_blocks = start_screen_description.size();
    m_width_pixels = m_width_blocks * BLOCK_SIZE + left_offset * 2;
    m_height_pixels = m_height_blocks * BLOCK_SIZE + bottom_offset * 2;

    m_startScreenElements.reserve(
        static_cast<size_t>(m_width_blocks * m_height_blocks));

    unsigned int current_offset_y =
        (m_height_blocks - 1) * BLOCK_SIZE + bottom_offset;
    for (const std::string& current_row : start_screen_description) {
      unsigned int current_offset_x = left_offset;
      for (const char current_row_element : current_row) {
        m_startScreenElements.emplace_back(
            std::make_pair<std::shared_ptr<RenderEngine::Sprite2D>, glm::vec2>(
                createStartScreenElementFromDescription(current_row_element),
                glm::vec2(current_offset_x, current_offset_y)));
        current_offset_x += BLOCK_SIZE;
      }
      current_offset_y -= BLOCK_SIZE;
    }

    // menu selections
    m_menu_selections.emplace_back(
        std::make_pair<std::shared_ptr<RenderEngine::Sprite2D>, glm::vec2>(
            Game::Instance().GetResourcesManager()->GetSprite(
                "One_Player_Selection"),
            glm::vec2(menu_position_x, menu_position_y)));

    m_menu_selections.emplace_back(
        std::make_pair<std::shared_ptr<RenderEngine::Sprite2D>, glm::vec2>(
            Game::Instance().GetResourcesManager()->GetSprite(
                "Two_Players_Selection"),
            glm::vec2(menu_position_x, menu_position_y - BLOCK_SIZE * 2)));

    // m_menu_selections.emplace_back(
    //     std::make_pair<std::shared_ptr<RenderEngine::Sprite2D>, glm::vec2>(
    //         Game::Instance().GetResourcesManager()->GetSprite("Construction_Selection"),
    //         glm::vec2(menu_position_x, menu_position_y - BLOCK_SIZE * 4)));

    // menu selector
    m_menu_selector =
        std::make_pair<std::shared_ptr<RenderEngine::Sprite2D>, glm::vec2>(
            Game::Instance().GetResourcesManager()->GetSprite(
                SELECTOR_SPRITE_NAME),
            glm::vec2(menu_position_x - BLOCK_SIZE * 3, menu_position_y));
  }
}

void StartScreen::setShaderProgram(
    std::shared_ptr<RenderEngine::ShaderProgram>&& shader_program) noexcept {
  m_start_srcreen_elements_shader_program = std::move(shader_program);
}

bool StartScreen::SetProjectiomMatrix() const noexcept {
  glm::mat4 projection_matrix = glm::ortho<float>(
      0.f, GetGameStateWidth(), 0.f, GetGameStateHeight(), -100.f, 100.f);

  if (m_start_srcreen_elements_shader_program) {
    m_start_srcreen_elements_shader_program->Use();
    m_start_srcreen_elements_shader_program->SetMatrix4("clip_matrix",
                                                        projection_matrix);
  } else {
    return false;
  }
  return true;
}

bool StartScreen::start() const noexcept { return SetProjectiomMatrix(); }

size_t StartScreen::GetGameStateWidth() const noexcept {
  return m_width_pixels;
}

size_t StartScreen::GetGameStateHeight() const noexcept {
  return m_height_pixels;
}

StartScreen::EMenuPuncts StartScreen::select() const noexcept {
  if (m_menu_selector.second.y == m_menu_selections[0].second.y) {
    return EMenuPuncts::LevelOnePlayer;
  } else if (m_menu_selector.second.y == m_menu_selections[1].second.y) {
    return EMenuPuncts::LevelTwoPlayers;
  } else {
    return EMenuPuncts::Constructor;
  }
}

void StartScreen::Update(double delta, KeyboardType& keyboard) {
  if (keyboard[GLFW_KEY_S] || keyboard[GLFW_KEY_DOWN]) {
    bool is_above_than_bottom_selection =
        m_menu_selector.second.y >
        m_menu_selections[m_menu_selections.size() - 1].second.y;
    m_menu_selector.second.y = is_above_than_bottom_selection
                                   ? m_menu_selector.second.y - BLOCK_SIZE * 2
                                   : m_menu_selections[0].second.y;
    keyboard[GLFW_KEY_S] = false;
    keyboard[GLFW_KEY_DOWN] = false;
  } else if (keyboard[GLFW_KEY_W] || keyboard[GLFW_KEY_UP]) {
    bool is_less_than_upper_selection =
        m_menu_selector.second.y < m_menu_selections[0].second.y;
    m_menu_selector.second.y =
        is_less_than_upper_selection
            ? m_menu_selector.second.y + BLOCK_SIZE * 2
            : m_menu_selections[m_menu_selections.size() - 1].second.y;
    keyboard[GLFW_KEY_W] = false;
    keyboard[GLFW_KEY_UP] = false;
  }
}

void StartScreen::Render() const {
  for (const auto& current_start_screen_element : m_startScreenElements) {
    if (current_start_screen_element.first) {
      current_start_screen_element.first->Render(
          current_start_screen_element.second, glm::vec2(BLOCK_SIZE), 0.f, 0.f);
    }
  }

  for (const auto& current_menu_selection : m_menu_selections) {
    if (current_menu_selection.first) {
      current_menu_selection.first->Render(
          current_menu_selection.second,
          glm::vec2(MENU_SELECTION_WIDTH, MENU_SELECTION_HEIGHT), 0.f, 0.f);
    }
  }

  if (m_menu_selector.first) {
    m_menu_selector.first->Render(m_menu_selector.second,
                                  glm::vec2(BLOCK_SIZE * 2), 0.f, 0.f);
  }
}
}  // namespace BatleCity