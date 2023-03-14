#pragma once
#ifndef LEVEL_H
#define LEVEL_H

#include <string>
#include <vector>
#include <memory>

#include <glm/vec2.hpp>

#include "GameObjects/IGameObject.h"

namespace BatleCity
{
	class Level
	{
	public:

		static constexpr unsigned int BLOCK_SIZE = 16;



		Level(const std::vector<std::string>& level_description);

		Level(const Level&) = delete;
		Level& operator=(const Level&) = delete;

		Level(Level&& other) noexcept;
		Level& operator=(Level&& _right) noexcept;

		size_t getLevelWidth() const;
		size_t getLevelHeight() const;
		inline const glm::vec2& getPlayer1Respawn() const { return m_player1_respawn; }
		inline const glm::vec2& getPlayer2Respawn() const { return m_player2_respawn; }
		inline const glm::vec2& getEnemy1Respawn()  const { return m_enemy1_respawn; }
		inline const glm::vec2& getEnemy2Respawn()  const { return m_enemy2_respawn; }
		inline const glm::vec2& getEnemy3Respawn()  const { return m_enemy3_respawn; }

		void update(const double delta);
		void render() const;

	private:

		unsigned int m_width = 0;
		unsigned int m_height = 0;

		glm::vec2 m_player1_respawn;
		glm::vec2 m_player2_respawn;
		glm::vec2 m_enemy1_respawn;
		glm::vec2 m_enemy2_respawn;
		glm::vec2 m_enemy3_respawn;

		std::vector<std::shared_ptr<IGameObject>> m_map_objects;

	};
}

#endif // !LEVEL_H
