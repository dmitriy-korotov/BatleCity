#pragma once
#ifndef ENEMY_TANK_AI_H
#define ENEMY_TANK_AI_H

#include <vector>
#include <string>
#include <memory>
#include <optional>



namespace BatleCity
{
	class EnemyTank;
	class Level;

	class EnemyTankAI
	{
	public:

		using Point = std::pair<size_t, size_t>;
		using Path = std::vector<Point>;



		EnemyTankAI(std::shared_ptr<const Level>) noexcept;
		EnemyTankAI(const EnemyTankAI&) = delete;
		EnemyTankAI& operator=(const EnemyTankAI&) = delete;
		EnemyTankAI(EnemyTankAI&&) = default;
		EnemyTankAI& operator=(EnemyTankAI&&) = default;

		void activeOnTank(std::shared_ptr<EnemyTank> enemy_tank) noexcept;
		void update(double delta) noexcept;

	private:

		std::optional<Point> findEaglePosition() const noexcept;
		std::optional<Path> calculatePathToEagle() const noexcept;
		std::pair<std::vector<std::vector<int64_t>>, Point> getFilledDynamic(const Point& tank_position, const std::optional<Point>& eagle_position) const noexcept;
		Path getPathFromDynamic(std::vector<std::vector<int64_t>>& dp, const Point& last_visited_point, const Point& tank_position) const noexcept;
		Point getIndexesTankPosition() const noexcept;

	private:

		static constexpr char EAGLE_SYMBOL = 'E';
		static constexpr char PERMITTED_FOR_PATH_SYMBOL = 'D';

	private:

		std::shared_ptr<const Level> m_level;
		std::shared_ptr<EnemyTank> m_enemy_tank = nullptr;

		std::optional<Point> m_eagle_position;
		std::optional<Path> m_path_to_eagle;
		size_t m_current_path_index = 0;

	};
}

#endif // ENEMY_TANK_AI_H
