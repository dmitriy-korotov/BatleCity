#pragma once
#ifndef ENEMY_TANK_AI_H
#define ENEMY_TANK_AI_H

#include <memory>
#include <optional>
#include <vector>

namespace BatleCity {
class EnemyTank;
class Level;

class EnemyTankAI {
 public:
  using Point = std::pair<size_t, size_t>;
  using Path = std::vector<Point>;

  EnemyTankAI(std::shared_ptr<const Level>) noexcept;
  EnemyTankAI(const EnemyTankAI&) = delete;
  EnemyTankAI& operator=(const EnemyTankAI&) = delete;
  EnemyTankAI(EnemyTankAI&&) = default;
  EnemyTankAI& operator=(EnemyTankAI&&) = default;

  void ActiveOnTank(std::shared_ptr<EnemyTank> enemy_tank) noexcept;
  void Update(double delta) noexcept;

 private:
  std::optional<Point> FindEaglePosition() const noexcept;
  std::optional<Path> CalculatePathToEagle() const noexcept;
  std::pair<std::vector<std::vector<int64_t>>, EnemyTankAI::Point>
  CalculateDistanceMap(const Point& start_pos) const noexcept;
  Path GetPathFromDynamic(std::vector<std::vector<int64_t>>& dp,
                          const Point& last_visited_point,
                          const Point& tank_position) const noexcept;
  Point GetIndexesTankPosition() const noexcept;
  EnemyTankAI::Path ReconstructPath(
      const std::vector<std::vector<int64_t>>& dist, const Point& start,
      const Point& end) const noexcept;

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
}  // namespace BatleCity

#endif  // ENEMY_TANK_AI_H
