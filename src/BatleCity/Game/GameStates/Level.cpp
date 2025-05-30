#include "Level.h"
#include <cstdlib>
#include <memory>
#include "glm/ext/vector_float2.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>

#include <BatleCity/Game/Game.h>

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
std::shared_ptr<RenderEngine::ShaderProgram> Level::m_gameObjectsShaderProgram =
    nullptr;
std::shared_ptr<RenderEngine::ShaderProgram> Level::m_collidersShaderProgram =
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
          m_enemyRespawn_1 = {current_offset_x, current_offset_y};
          break;
        case 'N':
          m_enemyRespawn_2 = {current_offset_x, current_offset_y};
          break;
        case 'O':
          m_enemyRespawn_3 = {current_offset_x, current_offset_y};
          break;
        default: {
          auto object = createGameObjectFromDescription(
              current_row_element,
              glm::vec2(current_offset_x, current_offset_y),
              glm::vec2(BLOCK_SIZE, BLOCK_SIZE), 0.f);
          if (object && object->GetGameObjectType() ==
                            IGameObject::EGameObjectType::Eagle) {
            m_eagle = std::static_pointer_cast<Eagle>(object);
          }
          m_staticObjectsMap.emplace_back(object);
          break;
        }
      }
      current_offset_x += BLOCK_SIZE;
    }
    current_offset_y -= BLOCK_SIZE;
  }

  // border bottom
  m_staticObjectsMap.emplace_back(std::make_shared<Border>(
      glm::vec2(BLOCK_SIZE, 0.f),
      glm::vec2(m_width_pixels, BOTTOM_BORDER_HEIGHT), 0.f, 0.f));

  // border top
  m_staticObjectsMap.emplace_back(std::make_shared<Border>(
      glm::vec2(BLOCK_SIZE, m_height_pixels + BLOCK_SIZE / 2.f),
      glm::vec2(m_width_pixels, TOP_BORDER_HEIGHT), 0.f, 0.f));

  // border left
  m_staticObjectsMap.emplace_back(std::make_shared<Border>(
      glm::vec2(0.f, 0.f),
      glm::vec2(LEFT_BORDER_WIDTH, BLOCK_SIZE * (m_height_blocks + 1)), 0.f,
      0.f));

  // // border right
  m_staticObjectsMap.emplace_back(std::make_shared<Border>(
      glm::vec2(BLOCK_SIZE * (m_width_blocks + 1), 0.f),
      glm::vec2(RIGHT_BORDER_WIDTH, BLOCK_SIZE * (m_height_blocks + 1)), 0.f,
      0.f));
}

Level::Level(std::vector<std::string>&& level_description)
    : IGameState(EGameStates::Level),
      m_description(std::move(level_description)) {
  if (!m_gameObjectsShaderProgram) {
    SetGameObjectsShaderProgram(
        Game::Instance().GetResourcesManager()->GetShaderProgram(
            GAME_OBJECTS_SHADER_PROGRAM_NAME));
  }
  if (!m_collidersShaderProgram) {
    SetCollidersShaderProgram(
        Game::Instance().GetResourcesManager()->GetShaderProgram(
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
    m_enemyRespawn_1 = {BLOCK_SIZE, m_height_pixels - BLOCK_SIZE / 2.f};
    m_enemyRespawn_2 = {(m_width_blocks / 2) * BLOCK_SIZE,
                        m_height_pixels - BLOCK_SIZE / 2.f};
    m_enemyRespawn_3 = {m_width_pixels, m_height_pixels - BLOCK_SIZE / 2.f};

    m_staticObjectsMap.reserve(
        static_cast<size_t>(m_width_blocks * m_height_blocks + 4));

    LoadMap();
  }

  m_game_over = Game::Instance().GetResourcesManager()->GetSprite("GameOver");
}

void Level::SetGameObjectsShaderProgram(
    std::shared_ptr<RenderEngine::ShaderProgram>&& shader_program) noexcept {
  m_gameObjectsShaderProgram = std::move(shader_program);
}

void Level::SetCollidersShaderProgram(
    std::shared_ptr<RenderEngine::ShaderProgram>&& shader_program) noexcept {
  m_collidersShaderProgram = std::move(shader_program);
}

void Level::SetLevelType(ELevelType level_type) noexcept {
  m_level_type = level_type;
}

const std::vector<std::string>& Level::GetLevelDescription() const noexcept {
  return m_description;
}

uint8_t Level::getLeftOffset() noexcept { return LEFT_BORDER_WIDTH; }

uint8_t Level::getRightOffset() noexcept { return RIGHT_BORDER_WIDTH; }

uint8_t Level::getBottomOffset() noexcept { return BOTTOM_BORDER_HEIGHT; }

uint8_t Level::getTopOffset() noexcept { return TOP_BORDER_HEIGHT; }

uint8_t Level::getBlockSize() noexcept { return BLOCK_SIZE; }

void Level::InitPhysics() const {
  if (m_player1) {
    Game::Instance().GetPhysicsEngine()->AddDynamicGameObject(m_player1);
  }
  if (m_player2) {
    Game::Instance().GetPhysicsEngine()->AddDynamicGameObject(m_player2);
  }
  try {
    Game::Instance().GetPhysicsEngine()->SetCurrentLevel(shared_from_this());
  } catch (const std::exception& ex) {
    std::cerr << "ERROR: shared_from_this exception (Level): " << ex.what()
              << std::endl;
  }
}

void Level::CreateTanks() const noexcept {
  m_player1.reset();
  m_player2.reset();
  switch (m_level_type) {
    case BatleCity::Level::ELevelType::TwoPlayers:
      m_player2 =
          std::make_shared<Tank>(Tank::ETankType::GreenTank5, m_player1_respawn,
                                 glm::vec2(BLOCK_SIZE, BLOCK_SIZE), 0.05f);
      [[fallthrough]];
    case BatleCity::Level::ELevelType::OnePlayer:
      m_player1 = std::make_shared<Tank>(
          Tank::ETankType::YellowTank1, m_player2_respawn,
          glm::vec2(BLOCK_SIZE, BLOCK_SIZE), 0.05f);
      break;
  }
}

void Level::CreateEnemyTank() const noexcept {
  try {
    auto spawnNumber = std::rand() % 3;
    glm::vec2 spawnPos;
    if (spawnNumber == 0) {
      spawnPos = m_enemyRespawn_1;
    } else if (spawnNumber == 1) {
      spawnPos = m_enemyRespawn_2;
    } else {
      spawnPos = m_enemyRespawn_3;
    }
    auto enemy = std::make_shared<EnemyTank>(
        shared_from_this(), Tank::ETankType::WhiteTank3, spawnPos,
        glm::vec2(BLOCK_SIZE, BLOCK_SIZE), 0.03f, 1000);
    enemy->Active();
    Game::Instance().GetPhysicsEngine()->AddDynamicGameObject(enemy);
    m_enemyTanks.emplace_back(std::move(enemy));
  } catch (const std::exception& ex) {
    std::cerr << "ERROR: Can't create enemy tank: " << ex.what() << std::endl;
  }
}

constexpr auto kRespawnDelay = 5000;

void Level::StartAI() const noexcept {
  auto creationCallback = [this]() {
    CreateEnemyTank();
    m_spawnEnemyTimer.Start(kRespawnDelay);
  };
  creationCallback();
  m_spawnEnemyTimer.SetCallBack(std::move(creationCallback));
}

void Level::SetProjectiomMatrix() const noexcept {
  assert(m_gameObjectsShaderProgram);
  assert(m_collidersShaderProgram);

  glm::mat4 projection_matrix = glm::ortho<float>(
      0.f, GetGameStateWidth(), 0.f, GetGameStateHeight(), -100.f, 100.f);

  m_gameObjectsShaderProgram->Use();
  m_gameObjectsShaderProgram->SetMatrix4("clip_matrix", projection_matrix);

  m_collidersShaderProgram->Use();
  m_collidersShaderProgram->SetMatrix4("clip_matrix", projection_matrix);
}

bool Level::start() const noexcept {
  // setup new game session
  if (!m_music) {
    m_music = std::make_shared<sf::Music>();
    if (!m_music->openFromFile(Game::Instance().GetResourcesManager()->GetResourcesPath() + "/res/music-2.wav")) {
      std::cerr << "ERROR: Can't open music file" << std::endl;
    }
  }
  m_music->setVolume(10);
  m_music->play();
  m_music->setLoop(true);
  LoadMap();
  SetProjectiomMatrix();
  CreateTanks();
  StartAI();
  InitPhysics();
  return true;
}

void Level::Reset() noexcept {
  m_isFinished = false;
  m_enemyTanks.clear();
  m_staticObjectsMap.clear();
  if (m_music && m_music->getStatus() == sf::Music::Playing) {
    m_music->stop();
  }
  Game::Instance().GetPhysicsEngine()->RemoveAllDynamicObjects();
}

size_t Level::GetGameStateWidth() const noexcept {
  return static_cast<size_t>(BLOCK_SIZE * (m_width_blocks + 3));
}

size_t Level::GetGameStateHeight() const noexcept {
  return static_cast<size_t>(BLOCK_SIZE * (m_height_blocks + 1));
}

std::vector<std::shared_ptr<BatleCity::IGameObject>> Level::GetObjectsFromArea(
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
          m_staticObjectsMap[static_cast<size_t>(y_index) * m_width_blocks +
                             x_index];
      if (object) {
        objects_in_area.emplace_back(object);
      }
    }
  }

  if (end_X == 13) {
    objects_in_area.emplace_back(
        m_staticObjectsMap[m_staticObjectsMap.size() - 1]);
  }
  if (start_Y == 0) {
    objects_in_area.emplace_back(
        m_staticObjectsMap[m_staticObjectsMap.size() - 3]);
  }
  if (start_X == 0) {
    objects_in_area.emplace_back(
        m_staticObjectsMap[m_staticObjectsMap.size() - 2]);
  }
  if (end_Y == 14) {
    objects_in_area.emplace_back(
        m_staticObjectsMap[m_staticObjectsMap.size() - 4]);
  }
  return objects_in_area;
}

void Level::UpdateTank(std::shared_ptr<Tank>& tank, KeyboardType& keyboard,
                       const std::vector<uint16_t>& tankActions2Keys) noexcept {
  if (keyboard[tankActions2Keys[static_cast<size_t>(ETankActions::MoveTop)]]) {
    tank->SetOrientation(IDynamicGameObject::EOrientation::Top);
    if (keyboard[tankActions2Keys[static_cast<size_t>(
            ETankActions::SlowDown)]]) {
      tank->SetVelocity(tank->GetMaxVelocity() / 2);
    } else {
      tank->SetVelocity(tank->GetMaxVelocity());
    }
  } else if (keyboard[tankActions2Keys[static_cast<size_t>(
                 ETankActions::MoveRight)]]) {
    tank->SetOrientation(IDynamicGameObject::EOrientation::Right);
    if (keyboard[tankActions2Keys[static_cast<size_t>(
            ETankActions::SlowDown)]]) {
      tank->SetVelocity(tank->GetMaxVelocity() / 2);
    } else {
      tank->SetVelocity(tank->GetMaxVelocity());
    }
  } else if (keyboard[tankActions2Keys[static_cast<size_t>(
                 ETankActions::MoveBottom)]]) {
    tank->SetOrientation(IDynamicGameObject::EOrientation::Bottom);
    if (keyboard[tankActions2Keys[static_cast<size_t>(
            ETankActions::SlowDown)]]) {
      tank->SetVelocity(tank->GetMaxVelocity() / 2);
    } else {
      tank->SetVelocity(tank->GetMaxVelocity());
    }
  } else if (keyboard[tankActions2Keys[static_cast<size_t>(
                 ETankActions::MoveLeft)]]) {
    tank->SetOrientation(IDynamicGameObject::EOrientation::Left);
    if (keyboard[tankActions2Keys[static_cast<size_t>(
            ETankActions::SlowDown)]]) {
      tank->SetVelocity(tank->GetMaxVelocity() / 2);
    } else {
      tank->SetVelocity(tank->GetMaxVelocity());
    }
  } else {
    tank->SetVelocity(0);
  }

  if (keyboard[tankActions2Keys[static_cast<size_t>(ETankActions::Fire)]]) {
    reinterpret_cast<const std::shared_ptr<Tank>&>(tank)->fair();
  }
}

void Level::UpdateStaticMapObjects(double delta) noexcept {
  for (const auto& object : m_staticObjectsMap) {
    if (object) {
      object->Update(delta);
    }
  }
}

void Level::UpdateDynamicMapObjects(double delta) noexcept {
  for (const auto& tank : m_enemyTanks) {
    if (tank) {
      tank->Update(delta);
    }
  }
  if (m_player1) {
    m_player1->Update(delta);
  }
  if (m_player2) {
    m_player2->Update(delta);
  }
}

void Level::Update(const double delta, KeyboardType& keyboard) {
  if (m_eagle && m_eagle->GetState() == Eagle::EEagleState::Dead) {
    m_isFinished = true;
  }
  if (m_player1 && m_player2) {
    m_isFinished |= m_player1->IsDestroy() && m_player2->IsDestroy();
  }
  if (m_player1 && !m_player2) {
    m_isFinished |= m_player1->IsDestroy();
  }
  if (m_player2 && !m_player1) {
    m_isFinished |= m_player2->IsDestroy();
  }
  if (m_isFinished) {
    return;
  }

  UpdateStaticMapObjects(delta);
  UpdateDynamicMapObjects(delta);

  if (keyboard[GLFW_KEY_F]) {
    IGameObject::EnableRenderingColliders();
  }
  if (keyboard[GLFW_KEY_G]) {
    IGameObject::DisableRenderingColliders();
  }

  switch (m_level_type) {
    case BatleCity::Level::ELevelType::TwoPlayers:
      UpdateTank(m_player2, keyboard, m_player2_keys);
      [[fallthrough]];
    case BatleCity::Level::ELevelType::OnePlayer:
      UpdateTank(m_player1, keyboard, m_player1_keys);
      break;
  }

  m_spawnEnemyTimer.Update(delta);

  Game::Instance().GetPhysicsEngine()->Update(delta);
}

void Level::Render() const {
  for (const auto& object : m_staticObjectsMap) {
    if (object) {
      object->Render();
      object->RenderColliders();
    }
  }
  for (const auto& tank : m_enemyTanks) {
    if (tank) {
      tank->Render();
      tank->RenderColliders();
    }
  }
  if (m_player1) {
    m_player1->Render();
    m_player1->RenderColliders();
  }
  if (m_player2) {
    m_player2->Render();
    m_player2->RenderColliders();
  }

  if (IsFinished()) {
    const auto size = glm::vec2(150.f, 150.f);
    const auto pos =
        glm::vec2(GetGameStateWidth(), GetGameStateHeight()) / 2.f - size / 2.f;
    m_game_over->Render(pos, size, 0.f, 5);
  }
}
}  // namespace BatleCity