#pragma once
#ifndef LEVEL_H
#define LEVEL_H

#include <glm/vec2.hpp>

#include "IGameState.h"

#include "../../Render/Sprite2D.h"
#include "../../System/Timer.h"
#include "../GameObjects/Eagle.h"

#include <memory>
#include <string>
#include <vector>

namespace BatleCity {
class IGameObject;
class IDynamicGameObject;
class Tank;

class Level : public IGameState, public std::enable_shared_from_this<Level> {
 public:
  enum class ELevelType : uint8_t { OnePlayer, TwoPlayers };

  Level(std::vector<std::string>&& level_description);
  ~Level() override{};

  Level(const Level&) = delete;
  Level& operator=(const Level&) = delete;

  Level(Level&&) = default;
  Level& operator=(Level&&) = default;

  size_t GetGameStateWidth() const noexcept override;
  size_t GetGameStateHeight() const noexcept override;
  const glm::vec2& getPlayer1Respawn() const { return m_player1_respawn; }
  const glm::vec2& getPlayer2Respawn() const { return m_player2_respawn; }
  const glm::vec2& getEnemy1Respawn() const { return m_enemyRespawn_1; }
  const glm::vec2& getEnemy2Respawn() const { return m_enemyRespawn_2; }
  const glm::vec2& getEnemy3Respawn() const { return m_enemyRespawn_3; }

  std::vector<std::shared_ptr<BatleCity::IGameObject>> GetObjectsFromArea(
      const glm::vec2& position, const glm::vec2& size) const;

  void SetLevelType(ELevelType level_type) noexcept;

  bool IsFinished() const noexcept { return m_isFinished; }

  bool start() const noexcept override;
  void Update(const double delta, KeyboardType& keyboard) override;
  void Render() const override;

  const std::vector<std::string>& GetLevelDescription() const noexcept;
  static uint8_t getLeftOffset() noexcept;
  static uint8_t getRightOffset() noexcept;
  static uint8_t getBottomOffset() noexcept;
  static uint8_t getTopOffset() noexcept;

  static uint8_t getBlockSize() noexcept;

 private:
  enum class ETankActions : uint8_t {
    MoveLeft = 0,
    MoveRight,
    MoveTop,
    MoveBottom,
    SlowDown,
    Fire
  };

  void LoadMap() const;
  void SetProjectiomMatrix() const noexcept;
  void InitPhysics() const;
  void StartAI() const noexcept;
  void CreateEnemyTank() const noexcept;
  void CreateTanks() const noexcept;

  void UpdateTank(std::shared_ptr<Tank>& tank, KeyboardType& keyboard,
                  const std::vector<uint16_t>& keys) noexcept;
  void UpdateStaticMapObjects(double delta) noexcept;
  void UpdateDynamicMapObjects(double delta) noexcept;

 private:
  static void SetGameObjectsShaderProgram(
      std::shared_ptr<RenderEngine::ShaderProgram>&& shader_program) noexcept;
  static void SetCollidersShaderProgram(
      std::shared_ptr<RenderEngine::ShaderProgram>&& shader_program) noexcept;

  static constexpr uint8_t BLOCK_SIZE = 16;
  static constexpr uint8_t LEFT_BORDER_WIDTH = BLOCK_SIZE;
  static constexpr uint8_t RIGHT_BORDER_WIDTH = BLOCK_SIZE * 2;
  static constexpr uint8_t BOTTOM_BORDER_HEIGHT = BLOCK_SIZE / 2;
  static constexpr uint8_t TOP_BORDER_HEIGHT = BLOCK_SIZE / 2;

  static std::shared_ptr<RenderEngine::ShaderProgram>
      m_gameObjectsShaderProgram;
  static std::shared_ptr<RenderEngine::ShaderProgram> m_collidersShaderProgram;

  static const std::vector<uint16_t> m_player1_keys;
  static const std::vector<uint16_t> m_player2_keys;

 private:
  ELevelType m_level_type = ELevelType::OnePlayer;

  std::vector<std::string> m_description;

  mutable glm::vec2 m_player1_respawn = glm::vec2(0.f);
  mutable glm::vec2 m_player2_respawn = glm::vec2(0.f);
  mutable glm::vec2 m_enemyRespawn_1 = glm::vec2(0.f);
  mutable glm::vec2 m_enemyRespawn_2 = glm::vec2(0.f);
  mutable glm::vec2 m_enemyRespawn_3 = glm::vec2(0.f);

  mutable std::vector<std::shared_ptr<IGameObject>> m_staticObjectsMap;

  mutable std::shared_ptr<Tank> m_player1 = nullptr;
  mutable std::shared_ptr<Tank> m_player2 = nullptr;
  mutable std::vector<std::shared_ptr<IDynamicGameObject>> m_enemyTanks;

  mutable System::Timer m_spawnEnemyTimer;
  mutable std::shared_ptr<RenderEngine::Sprite2D> m_game_over = nullptr;
  mutable std::shared_ptr<Eagle> m_eagle = nullptr;
  mutable bool m_isFinished = false;
};
}  // namespace BatleCity

#endif  // !LEVEL_H
