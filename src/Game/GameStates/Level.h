#pragma once
#ifndef LEVEL_H
#define LEVEL_H

#include <glm/vec2.hpp>

#include "IGameState.h"

#include <string>
#include <vector>
#include <memory>



namespace BatleCity
{
	class IGameObject;
	class IDynamicGameObject;
	class Tank;

	class Level : public IGameState, public std::enable_shared_from_this<Level>
	{
	public:

		enum class ELevelType : uint8_t
		{
			OnePlayer,
			TwoPlayers
		};



		Level(std::vector<std::string>&& level_description);
		~Level() override { };

		Level(const Level&) = delete;
		Level& operator=(const Level&) = delete;

		Level(Level&&) = default;
		Level& operator=(Level&&) = default;

		size_t getGameStateWidth() const noexcept override;
		size_t getGameStateHeight() const noexcept override;
		const glm::vec2& getPlayer1Respawn() const { return m_player1_respawn; }
		const glm::vec2& getPlayer2Respawn() const { return m_player2_respawn; }
		const glm::vec2& getEnemy1Respawn()  const { return m_enemy1_respawn; }
		const glm::vec2& getEnemy2Respawn()  const { return m_enemy2_respawn; }
		const glm::vec2& getEnemy3Respawn()  const { return m_enemy3_respawn; }

		std::vector<std::shared_ptr<BatleCity::IGameObject>> getObjectsFromArea(const glm::vec2& position, const glm::vec2& size) const;

		void setLevelType(ELevelType level_type) noexcept;

		bool start() const noexcept override;
		void update(const double delta, std::array<bool, 349>& keyboard) override;
		void render() const override;

		const std::vector<std::string>& getLevelDescription() const noexcept;
		static uint8_t getLeftOffset() noexcept;
		static uint8_t getRightOffset() noexcept;
		static uint8_t getBottomOffset() noexcept;
		static uint8_t getTopOffset() noexcept;

		static uint8_t getBlockSize() noexcept;

	private:

		enum class ETankActions : uint8_t
		{
			MoveLeft = 0,
			MoveRight,
			MoveTop,
			MoveBottom,
			SlowDown,
			Fire
		};


		bool setProjectiomMatrix() const noexcept;
		void initPhysics() const;
		void startAI() const noexcept;
		void createEnemyTanks() const noexcept;
		void createTanks() const noexcept;

		void updateTank(std::shared_ptr<Tank>& tank, std::array<bool, 349>& keyboard, const std::vector<uint16_t>& keys) noexcept;
		void updateStaticMapObjects(double delta) noexcept;
		void updateDynamicMapObjects(double delta) noexcept;

	private:

		static void setGameObjectsShaderProgram(std::shared_ptr<RenderEngine::ShaderProgram>& shader_program) noexcept;
		static void setCollidersShaderProgram(std::shared_ptr<RenderEngine::ShaderProgram>& shader_program) noexcept;

		static constexpr uint8_t BLOCK_SIZE = 16;
		static constexpr uint8_t LEFT_BORDER_WIDTH = BLOCK_SIZE;
		static constexpr uint8_t RIGHT_BORDER_WIDTH = BLOCK_SIZE * 2;
		static constexpr uint8_t BOTTOM_BORDER_HEIGHT = BLOCK_SIZE / 2;
		static constexpr uint8_t TOP_BORDER_HEIGHT = BLOCK_SIZE / 2;

		static std::shared_ptr<RenderEngine::ShaderProgram> m_game_obgects_shader_program;
		static std::shared_ptr<RenderEngine::ShaderProgram> m_colliders_shader_program;

		static const std::vector<uint16_t> m_player1_keys;
		static const std::vector<uint16_t> m_player2_keys;

	private:

		ELevelType m_level_type = ELevelType::OnePlayer;

		std::vector<std::string> m_description;

		glm::vec2 m_player1_respawn = glm::vec2(0.f);
		glm::vec2 m_player2_respawn = glm::vec2(0.f);
		glm::vec2 m_enemy1_respawn  = glm::vec2(0.f);
		glm::vec2 m_enemy2_respawn  = glm::vec2(0.f);
		glm::vec2 m_enemy3_respawn  = glm::vec2(0.f);

		std::vector<std::shared_ptr<IGameObject>> m_static_map_objects;


		mutable std::shared_ptr<Tank> m_player1 = nullptr;
		mutable std::shared_ptr<Tank> m_player2 = nullptr;
		mutable std::vector<std::shared_ptr<IDynamicGameObject>> m_enemy_tanks;

	};
}

#endif // !LEVEL_H
