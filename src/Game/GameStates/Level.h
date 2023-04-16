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

	class Level : public IGameState, private std::enable_shared_from_this<Level>
	{
	public:

		static constexpr unsigned int BLOCK_SIZE = 16;




		Level(const std::vector<std::string>& level_description);
		~Level() override { };

		Level(const Level&) = delete;
		Level& operator=(const Level&) = delete;

		size_t getGameStateWidth() const noexcept override;
		size_t getGameStateHeight() const noexcept override;
		inline const glm::vec2& getPlayer1Respawn() const { return m_player1_respawn; }
		inline const glm::vec2& getPlayer2Respawn() const { return m_player2_respawn; }
		inline const glm::vec2& getEnemy1Respawn()  const { return m_enemy1_respawn; }
		inline const glm::vec2& getEnemy2Respawn()  const { return m_enemy2_respawn; }
		inline const glm::vec2& getEnemy3Respawn()  const { return m_enemy3_respawn; }

		std::vector<std::shared_ptr<BatleCity::IGameObject>> getObjectsFromArea(const glm::vec2& position, const glm::vec2& size) const;

		bool start() const noexcept override;
		void initPhysics() const;
		void update(const double delta, std::array<bool, 349>& keyboard) override;
		void render() const override;

	private:

		bool setProjectiomMatrix() const noexcept;

		static void setGameObjectsShaderProgram(std::shared_ptr<RenderEngine::ShaderProgram>& shader_program) noexcept;
		static void setCollidersShaderProgram(std::shared_ptr<RenderEngine::ShaderProgram>& shader_program) noexcept;

		static std::shared_ptr<RenderEngine::ShaderProgram> m_game_obgects_shader_program;
		static std::shared_ptr<RenderEngine::ShaderProgram> m_colliders_shader_program;

		glm::vec2 m_player1_respawn = glm::vec2(0.f);
		glm::vec2 m_player2_respawn = glm::vec2(0.f);
		glm::vec2 m_enemy1_respawn  = glm::vec2(0.f);
		glm::vec2 m_enemy2_respawn  = glm::vec2(0.f);
		glm::vec2 m_enemy3_respawn  = glm::vec2(0.f);

		std::vector<std::shared_ptr<IGameObject>> m_static_map_objects;
		std::vector<std::shared_ptr<IDynamicGameObject>> m_dynamic_map_objects;

	};
}

#endif // !LEVEL_H
