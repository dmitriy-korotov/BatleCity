#include "Level.h"
#include <cstdlib>
#include <memory>
#include "glm/ext/vector_float2.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>

#include "../GameObjects/BetonWall.h"
#include "../GameObjects/Border.h"
#include "../GameObjects/BrickWall.h"
#include "../GameObjects/Eagle.h"
#include "../GameObjects/EnemyTank.h"
#include "../GameObjects/Ice.h"
#include "../GameObjects/Tank.h"
#include "../GameObjects/Tree.h"
#include "../GameObjects/Water.h"

#include "../../Physics/PhysicsEngine.h"

#include "../../Resources/ResourceManager.h"

#include <algorithm>
#include <iostream>

static constexpr const char* GAME_OBJECTS_SHADER_PROGRAM_NAME =
    "spriteShaderProgram";
static constexpr const char* COLLIDERS_SHADER_PROGRAM_NAME =
    "colliderShaderProgram";

static std::shared_ptr<BatleCity::IGameObject> createGameObjectFromDescription(
    const char description, const glm::vec2& position, const glm::vec2& size,
    const float rotation) {
  switch (description) {
    case '0':
      return std::make_shared<BatleCity::BrickWall>(
          BatleCity::Block::EBlockType::Right, position, size, rotation, 0.f);
      break;
    case '1':
      return std::make_shared<BatleCity::BrickWall>(
          BatleCity::Block::EBlockType::Bottom, position, size, rotation, 0.f);
      break;
    case '2':
      return std::make_shared<BatleCity::BrickWall>(
          BatleCity::Block::EBlockType::Left, position, size, rotation, 0.f);
      break;
    case '3':
      return std::make_shared<BatleCity::BrickWall>(
          BatleCity::Block::EBlockType::Top, position, size, rotation, 0.f);
      break;
    case '4':
      return std::make_shared<BatleCity::BrickWall>(
          BatleCity::Block::EBlockType::All, position, size, rotation, 0.f);
      break;
    case 'H':
      return std::make_shared<BatleCity::BrickWall>(
          BatleCity::Block::EBlockType::RightBottom, position, size, rotation,
          0.f);
      break;
    case 'G':
      return std::make_shared<BatleCity::BrickWall>(
          BatleCity::Block::EBlockType::LeftBottom, position, size, rotation,
          0.f);
      break;
    case '5':
      return std::make_shared<BatleCity::BetonWall>(
          BatleCity::Block::EBlockType::Right, position, size, rotation, 0.f);
      break;
    case '6':
      return std::make_shared<BatleCity::BetonWall>(
          BatleCity::Block::EBlockType::Bottom, position, size, rotation, 0.f);
      break;
    case '7':
      return std::make_shared<BatleCity::BetonWall>(
          BatleCity::Block::EBlockType::Left, position, size, rotation, 0.f);
      break;
    case '8':
      return std::make_shared<BatleCity::BetonWall>(
          BatleCity::Block::EBlockType::Top, position, size, rotation, 0.f);
      break;
    case '9':
      return std::make_shared<BatleCity::BetonWall>(
          BatleCity::Block::EBlockType::All, position, size, rotation, 0.f);
      break;
    case 'A':
      return std::make_shared<BatleCity::Water>(
          BatleCity::Block::EBlockType::All, position, size, rotation, -1.f);
      break;
    case 'B':
      return std::make_shared<BatleCity::Tree>(
          BatleCity::Block::EBlockType::All, position, size, rotation, 1.f);
      break;
    case 'C':
      return std::make_shared<BatleCity::Ice>(BatleCity::Block::EBlockType::All,
                                              position, size, rotation, -1.f);
      break;
    case 'D':
      return nullptr;
      break;
    case 'E':
      return std::make_shared<BatleCity::Eagle>(position, size, rotation, 0.f);
      break;
    default:
      std::cerr << "ERROR: Game object with such description not found: "
                << description << std::endl;
      return nullptr;
  }
}

namespace BatleCity {
std::shared_ptr<RenderEngine::ShaderProgram>
    Level::m_game_obgects_shader_program = nullptr;
std::shared_ptr<RenderEngine::ShaderProgram> Level::m_colliders_shader_program =
    nullptr;

const std::vector<uint16_t> Level::m_player1_keys = {
    GLFW_KEY_A, GLFW_KEY_D,          GLFW_KEY_W,
    GLFW_KEY_S, GLFW_KEY_LEFT_SHIFT, GLFW_KEY_SPACE};
const std::vector<uint16_t> Level::m_player2_keys = {
    GLFW_KEY_LEFT, GLFW_KEY_RIGHT,       GLFW_KEY_UP,
    GLFW_KEY_DOWN, GLFW_KEY_RIGHT_SHIFT, GLFW_KEY_ENTER};

void Level::LoadMap() const {
  unsigned int current_offset_y = (m_height_blocks - 0.5f) * BLOCK_SIZE;
  for (const std::string& current_row : m_description) {
    unsigned int current_offset_x = BLOCK_SIZE;
    for (const char current_row_element : current_row) {
      switch (current_row_element) {
        case 'K':
          m_player1_respawn = {current_offset_x, current_offset_y};
          break;
        case 'L':
          m_player2_respawn = {current_offset_x, current_offset_y};
          break;
        case 'M':
          m_enemy1_respawn = {current_offset_x, current_offset_y};
          break;
        case 'N':
          m_enemy2_respawn = {current_offset_x, current_offset_y};
          break;
        case 'O':
          m_enemy3_respawn = {current_offset_x, current_offset_y};
          break;
        default: {
          auto object = createGameObjectFromDescription(
                  current_row_element,
                  glm::vec2(current_offset_x, current_offset_y),
                  glm::vec2(BLOCK_SIZE, BLOCK_SIZE), 0.f);
          if (object && object->getGameObjectType() == IGameObject::EGameObjectType::Eagle) {
            m_eagle = std::static_pointer_cast<Eagle>(object);
          }
          m_static_map_objects.emplace_back(object);
          break;
        }
      }
      current_offset_x += BLOCK_SIZE;
    }
    current_offset_y -= BLOCK_SIZE;
  }

  // border bottom
  m_static_map_objects.emplace_back(std::make_shared<Border>(
      glm::vec2(BLOCK_SIZE, 0.f),
      glm::vec2(m_width_pixels, BOTTOM_BORDER_HEIGHT), 0.f, 0.f));

  // border top
  m_static_map_objects.emplace_back(std::make_shared<Border>(
      glm::vec2(BLOCK_SIZE, m_height_pixels + BLOCK_SIZE / 2.f),
      glm::vec2(m_width_pixels, TOP_BORDER_HEIGHT), 0.f, 0.f));

  // border left
  m_static_map_objects.emplace_back(std::make_shared<Border>(
      glm::vec2(0.f, 0.f),
      glm::vec2(LEFT_BORDER_WIDTH, BLOCK_SIZE * (m_height_blocks + 1)), 0.f,
      0.f));

  // // border right
  m_static_map_objects.emplace_back(std::make_shared<Border>(
      glm::vec2(BLOCK_SIZE * (m_width_blocks + 1), 0.f),
      glm::vec2(RIGHT_BORDER_WIDTH, BLOCK_SIZE * (m_height_blocks + 1)), 0.f,
      0.f));
}

Level::Level(std::vector<std::string>&& level_description)
    : IGameState(EGameStates::Level),
      m_description(std::move(level_description)) {
  if (!m_game_obgects_shader_program) {
    setGameObjectsShaderProgram(Resources::ResourceManager::getShaderProgram(
        GAME_OBJECTS_SHADER_PROGRAM_NAME));
  }
  if (!m_colliders_shader_program) {
    setCollidersShaderProgram(Resources::ResourceManager::getShaderProgram(
        COLLIDERS_SHADER_PROGRAM_NAME));
  }

  if (m_description.empty()) {
    std::cerr << "Level description is empty" << std::endl;
  } else {
    m_width_blocks = m_description[0].length();
    m_height_blocks = m_description.size();
    m_width_pixels = m_width_blocks * BLOCK_SIZE;
    m_height_pixels = m_height_blocks * BLOCK_SIZE;

    m_player1_respawn = {(m_width_blocks / 2 - 1) * BLOCK_SIZE,
                         BLOCK_SIZE / 2.f};
    m_player2_respawn = {(m_width_blocks / 2 + 3) * BLOCK_SIZE,
                         BLOCK_SIZE / 2.f};
    m_enemy1_respawn = {BLOCK_SIZE, m_height_pixels - BLOCK_SIZE / 2.f};
    m_enemy2_respawn = {(m_width_blocks / 2) * BLOCK_SIZE,
                        m_height_pixels - BLOCK_SIZE / 2.f};
    m_enemy3_respawn = {m_width_pixels, m_height_pixels - BLOCK_SIZE / 2.f};

    m_static_map_objects.reserve(
        static_cast<size_t>(m_width_blocks * m_height_blocks + 4));

    LoadMap();
  }

  m_game_over = Resources::ResourceManager::getSprite("GameOver");
}

void Level::setGameObjectsShaderProgram(
    std::shared_ptr<RenderEngine::ShaderProgram>&& shader_program) noexcept {
  m_game_obgects_shader_program = std::move(shader_program);
}

void Level::setCollidersShaderProgram(
    std::shared_ptr<RenderEngine::ShaderProgram>&& shader_program) noexcept {
  m_colliders_shader_program = std::move(shader_program);
}

void Level::setLevelType(ELevelType level_type) noexcept {
  m_level_type = level_type;
}

const std::vector<std::string>& Level::getLevelDescription() const noexcept {
  return m_description;
}

uint8_t Level::getLeftOffset() noexcept { return LEFT_BORDER_WIDTH; }

uint8_t Level::getRightOffset() noexcept { return RIGHT_BORDER_WIDTH; }

uint8_t Level::getBottomOffset() noexcept { return BOTTOM_BORDER_HEIGHT; }

uint8_t Level::getTopOffset() noexcept { return TOP_BORDER_HEIGHT; }

uint8_t Level::getBlockSize() noexcept { return BLOCK_SIZE; }

void Level::initPhysics() const {
  std::for_each(
      m_enemy_tanks.cbegin(), m_enemy_tanks.cend(),
      [](std::shared_ptr<IDynamicGameObject> dynamic_game_object) -> void {
        Physics::PhysicsEngine::addDynamicGameObject(
            std::move(dynamic_game_object));
      });
  if (m_player1) {
    Physics::PhysicsEngine::addDynamicGameObject(m_player1);
  }
  if (m_player2) {
    Physics::PhysicsEngine::addDynamicGameObject(m_player2);
  }

  try {
    Physics::PhysicsEngine::setCurrentLevel(shared_from_this());
  } catch (const std::exception& ex) {
    std::cerr << "ERROR: shared_from_this exception (Level):" << std::endl;
    std::cerr << ex.what() << std::endl;
  }
}

void Level::createTanks() const noexcept {
  m_player1.reset();
  m_player2.reset();
  switch (m_level_type) {
    case BatleCity::Level::ELevelType::TwoPlayers:
      m_player2 =
          std::make_shared<Tank>(Tank::ETankType::GreenTank5, m_player1_respawn,
                                 glm::vec2(BLOCK_SIZE, BLOCK_SIZE), 0.05f);
      [[fallthrough]];
    case BatleCity::Level::ELevelType::OnePlayer:
      m_player1 =
          std::make_shared<Tank>(Tank::ETankType::YellowTank1, m_player2_respawn,
                                 glm::vec2(BLOCK_SIZE, BLOCK_SIZE), 0.05f);
      break;
  }
}

void Level::createEnemyTanks() const noexcept {
  try {
    auto spawn_number = std::rand() % 3;
    glm::vec2 spawn_pos;
    if (spawn_number == 0) {
      spawn_pos = m_enemy1_respawn;
    } else if (spawn_number == 1) {
      spawn_pos = m_enemy2_respawn;
    } else {
      spawn_pos = m_enemy3_respawn;
    }
    auto enemy = std::make_shared<EnemyTank>(
        shared_from_this(), Tank::ETankType::WhiteTank3, spawn_pos,
        glm::vec2(BLOCK_SIZE, BLOCK_SIZE), 0.03f, 1000);
    enemy->active();
    Physics::PhysicsEngine::addDynamicGameObject(enemy);
    m_enemy_tanks.emplace_back(std::move(enemy));
  } catch (const std::exception& ex) {
    std::cerr << "ERROR: Can't create enemy tanks: " << ex.what() << std::endl;
  }
}

void Level::startAI() const noexcept {
  auto callback = [this]() {
    createEnemyTanks();
    m_spawn_enemy_timer.Start(5000);
  };
  callback();
  m_spawn_enemy_timer.SetCallBack(std::move(callback));
}

bool Level::setProjectiomMatrix() const noexcept {
  glm::mat4 projection_matrix = glm::ortho<float>(
      0.f, getGameStateWidth(), 0.f, getGameStateHeight(), -100.f, 100.f);

  if (m_game_obgects_shader_program) {
    m_game_obgects_shader_program->use();
    m_game_obgects_shader_program->setMatrix4("clip_matrix", projection_matrix);
  } else {
    return false;
  }

  if (m_colliders_shader_program) {
    m_colliders_shader_program->use();
    m_colliders_shader_program->setMatrix4("clip_matrix", projection_matrix);
  } else {
    return false;
  }

  return true;
}

bool Level::start() const noexcept {
  m_enemy_tanks.clear();
  m_static_map_objects.clear();
  LoadMap();
  Physics::PhysicsEngine::removeAllDynamicObjects();
  m_is_finished = false;
  if (!setProjectiomMatrix()) {
    return false;
  }
  createTanks();  //	1
  startAI();      //	2		this ordering really important
  initPhysics();  //	3
  return true;
}

size_t Level::getGameStateWidth() const noexcept {
  return static_cast<size_t>(BLOCK_SIZE * (m_width_blocks + 3));
}

size_t Level::getGameStateHeight() const noexcept {
  return static_cast<size_t>(BLOCK_SIZE * (m_height_blocks + 1));
}

std::vector<std::shared_ptr<BatleCity::IGameObject>> Level::getObjectsFromArea(
    const glm::vec2& position, const glm::vec2& size) const {
  std::vector<std::shared_ptr<BatleCity::IGameObject>> objects_in_area;

  glm::vec2 convent_position(
      std::clamp(position.x - BLOCK_SIZE, 0.f,
                 static_cast<float>(m_width_pixels)),
      std::clamp(m_height_pixels - position.y - BLOCK_SIZE / 2, 0.f,
                 static_cast<float>(m_height_pixels)));

  unsigned int start_X = static_cast<unsigned int>(std::floor(
      std::clamp(convent_position.x, 0.f, static_cast<float>(m_width_pixels)) /
      BLOCK_SIZE));
  unsigned int end_X = static_cast<unsigned int>(
      std::ceil(std::clamp(convent_position.x + size.x, 0.f,
                           static_cast<float>(m_width_pixels)) /
                BLOCK_SIZE));

  unsigned int start_Y = static_cast<unsigned int>(std::floor(
      std::clamp(convent_position.y, 0.f, static_cast<float>(m_height_pixels)) /
      BLOCK_SIZE));
  unsigned int end_Y = static_cast<unsigned int>(
      std::ceil((std::clamp(convent_position.y + size.y, 0.f,
                            static_cast<float>(m_height_pixels)) /
                 BLOCK_SIZE)));

  for (unsigned int x_index = start_X; x_index < end_X; ++x_index) {
    for (unsigned int y_index = start_Y; y_index < end_Y; ++y_index) {
      const auto& object =
          m_static_map_objects[static_cast<size_t>(y_index) * m_width_blocks +
                               x_index];
      if (object) {
        objects_in_area.emplace_back(object);
      }
    }
  }

  if (end_X == 13) {
    objects_in_area.emplace_back(
        m_static_map_objects[m_static_map_objects.size() - 1]);
  }
  if (start_Y == 0) {
    objects_in_area.emplace_back(
        m_static_map_objects[m_static_map_objects.size() - 3]);
  }
  if (start_X == 0) {
    objects_in_area.emplace_back(
        m_static_map_objects[m_static_map_objects.size() - 2]);
  }
  if (end_Y == 14) {
    objects_in_area.emplace_back(
        m_static_map_objects[m_static_map_objects.size() - 4]);
  }
  return objects_in_area;
}

void Level::updateTank(
    std::shared_ptr<Tank>& tank, std::array<bool, 349>& keyboard,
    const std::vector<uint16_t>& keys_for_this_tank_actions) noexcept {
  if (keyboard[keys_for_this_tank_actions[static_cast<size_t>(
          ETankActions::MoveTop)]]) {
    tank->setOrientation(IDynamicGameObject::EOrientation::Top);
    if (keyboard[keys_for_this_tank_actions[static_cast<size_t>(
            ETankActions::SlowDown)]]) {
      tank->setVelocity(tank->getMaxVelocity() / 2);
    } else {
      tank->setVelocity(tank->getMaxVelocity());
    }
  } else if (keyboard[keys_for_this_tank_actions[static_cast<size_t>(
                 ETankActions::MoveRight)]]) {
    tank->setOrientation(IDynamicGameObject::EOrientation::Right);
    if (keyboard[keys_for_this_tank_actions[static_cast<size_t>(
            ETankActions::SlowDown)]]) {
      tank->setVelocity(tank->getMaxVelocity() / 2);
    } else {
      tank->setVelocity(tank->getMaxVelocity());
    }
  } else if (keyboard[keys_for_this_tank_actions[static_cast<size_t>(
                 ETankActions::MoveBottom)]]) {
    tank->setOrientation(IDynamicGameObject::EOrientation::Bottom);
    if (keyboard[keys_for_this_tank_actions[static_cast<size_t>(
            ETankActions::SlowDown)]]) {
      tank->setVelocity(tank->getMaxVelocity() / 2);
    } else {
      tank->setVelocity(tank->getMaxVelocity());
    }
  } else if (keyboard[keys_for_this_tank_actions[static_cast<size_t>(
                 ETankActions::MoveLeft)]]) {
    tank->setOrientation(IDynamicGameObject::EOrientation::Left);
    if (keyboard[keys_for_this_tank_actions[static_cast<size_t>(
            ETankActions::SlowDown)]]) {
      tank->setVelocity(tank->getMaxVelocity() / 2);
    } else {
      tank->setVelocity(tank->getMaxVelocity());
    }
  } else {
    tank->setVelocity(0);
  }

  if (keyboard[keys_for_this_tank_actions[static_cast<size_t>(
          ETankActions::Fire)]]) {
    reinterpret_cast<const std::shared_ptr<Tank>&>(tank)->fair();
  }
}

void Level::updateStaticMapObjects(double delta) noexcept {
  for (const auto& current_static_map_object : m_static_map_objects) {
    if (current_static_map_object) {
      current_static_map_object->update(delta);
    }
  }
}

void Level::updateDynamicMapObjects(double delta) noexcept {
  for (const auto& current_dynamic_map_object : m_enemy_tanks) {
    if (current_dynamic_map_object) {
      current_dynamic_map_object->update(delta);
    }
  }

  if (m_player1) {
    m_player1->update(delta);
  }
  if (m_player2) {
    m_player2->update(delta);
  }
}

void Level::update(const double delta, std::array<bool, 349>& keyboard) {
  if (m_eagle && m_eagle->getState() == Eagle::EEagleState::Dead) {
    m_is_finished = true;
  }
  if (m_player1 && m_player2) {
    m_is_finished |= m_player1->isDestroy() && m_player2->isDestroy();
  }
  if (m_player1 && !m_player2) {
    m_is_finished |= m_player1->isDestroy();
  }
  if (m_player2 && !m_player1) {
    m_is_finished |= m_player2->isDestroy();
  }
  if (m_is_finished) {
    return;
  }

  updateStaticMapObjects(delta);
  updateDynamicMapObjects(delta);

  if (keyboard[GLFW_KEY_F]) {
    IGameObject::enableRenderingColliders();
  }
  if (keyboard[GLFW_KEY_G]) {
    IGameObject::disableRenderingColliders();
  }

  switch (m_level_type) {
    case BatleCity::Level::ELevelType::TwoPlayers:
      updateTank(m_player2, keyboard, m_player2_keys);
      [[fallthrough]];
    case BatleCity::Level::ELevelType::OnePlayer:
      updateTank(m_player1, keyboard, m_player1_keys);
      break;
  }

  m_spawn_enemy_timer.Update(delta);

  Physics::PhysicsEngine::update(delta);
}

void Level::render() const {
  for (const auto& current_static_map_object : m_static_map_objects) {
    if (current_static_map_object) {
      current_static_map_object->render();
      current_static_map_object->renderColliders();
    }
  }
  for (const auto& current_dynamic_map_object : m_enemy_tanks) {
    if (current_dynamic_map_object) {
      current_dynamic_map_object->render();
      current_dynamic_map_object->renderColliders();
    }
  }

  if (m_player1) {
    m_player1->render();
    m_player1->renderColliders();
  }
  if (m_player2) {
    m_player2->render();
    m_player2->renderColliders();
  }

  if (m_is_finished) {
    auto size = glm::vec2(150.f, 150.f);
    auto pos = glm::vec2(getGameStateWidth(), getGameStateHeight()) / 2.f - size / 2.f;
    m_game_over->render(pos, size, 0.f, 5);
  }
}
}  // namespace BatleCity