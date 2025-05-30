#include "EnemyTankAI.h"
#include <algorithm>
#include <climits>
#include <iostream>
#include <queue>
#include <random>
#include "../GameObjects/EnemyTank.h"
#include "../GameStates/Level.h"

namespace BatleCity {

EnemyTankAI::EnemyTankAI(std::shared_ptr<const Level> level_ptr) noexcept
    : m_level(std::move(level_ptr)) {}

void EnemyTankAI::ActiveOnTank(std::shared_ptr<EnemyTank> enemy_tank) noexcept {
  std::swap(m_enemy_tank, enemy_tank);
  m_eagle_position = FindEaglePosition();
  if (!m_eagle_position) {
    std::cerr << "ERROR: Can't find eagle" << std::endl;
  }
  m_path_to_eagle = CalculatePathToEagle();

  // if (m_path_to_eagle.has_value()) {
  //     for (const auto& point : *m_path_to_eagle) {
  //         std::cout << '(' << point.first / 16 << ", " << point.second / 16
  //         << ')' << std::endl;
  //     }
  // } else {
  //   std::cerr << "ERROR: Can't find path to eagle" << std::endl;
  // }
}

std::optional<EnemyTankAI::Point> EnemyTankAI::FindEaglePosition()
    const noexcept {
  const auto& level = m_level->GetLevelDescription();
  for (uint16_t y = 0; y < level.size(); ++y) {
    for (uint16_t x = 0; x < level[y].size(); ++x) {
      if (level[y][x] == EAGLE_SYMBOL) {
        return Point{x, y};
      }
    }
  }
  return std::nullopt;
}

std::optional<EnemyTankAI::Path> EnemyTankAI::CalculatePathToEagle()
    const noexcept {
  if (!m_eagle_position) return std::nullopt;

  Point tank_pos = GetIndexesTankPosition();
  auto [distance_map, last_point] = CalculateDistanceMap(tank_pos);

  if (distance_map[m_eagle_position->second][m_eagle_position->first] ==
      INT_MAX) {
    return std::nullopt;
  }

  return ReconstructPath(distance_map, tank_pos, *m_eagle_position);
}

EnemyTankAI::Point EnemyTankAI::GetIndexesTankPosition() const noexcept {
  float block_size = m_level->getBlockSize();
  float x =
      (m_enemy_tank->GetPosition().x - m_level->getLeftOffset()) / block_size;
  float y = (m_level->GetGameStateHeight() - m_level->getTopOffset() -
             m_enemy_tank->SetSize().y - m_enemy_tank->GetPosition().y) /
            block_size;
  return {static_cast<int>(std::round(x)), static_cast<int>(std::round(y))};
}

std::pair<std::vector<std::vector<int64_t>>, EnemyTankAI::Point>
EnemyTankAI::CalculateDistanceMap(const Point& start_pos) const noexcept {
  const auto& level = m_level->GetLevelDescription();
  size_t height = level.size();
  size_t width = level[0].size();

  std::vector<std::vector<int64_t>> dist(height,
                                         std::vector<int64_t>(width, INT_MAX));
  std::vector<std::vector<Point>> prev(height,
                                       std::vector<Point>(width, {-1, -1}));
  std::queue<Point> queue;

  dist[start_pos.second][start_pos.first] = 0;
  queue.push(start_pos);

  const std::vector<Point> directions = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
  Point last_point = start_pos;

  while (!queue.empty()) {
    Point current = queue.front();
    queue.pop();
    last_point = current;

    // Если достигли орла, прерываем поиск
    if (level[current.second][current.first] == EAGLE_SYMBOL) {
      break;
    }

    for (const auto& dir : directions) {
      Point next = {current.first + dir.first, current.second + dir.second};

      if (next.first >= 0 && next.first < width && next.second >= 0 &&
          next.second < height) {
        bool is_passable = level[next.second][next.first] != '8' &&
                           level[next.second][next.first] != '9' &&
                           level[next.second][next.first] != 'A';

        if (is_passable && dist[next.second][next.first] == INT_MAX) {
          int weight = 1;
          if (level[next.second][next.first] != PERMITTED_FOR_PATH_SYMBOL) {
            weight += 1;
          }
          dist[next.second][next.first] =
              dist[current.second][current.first] + weight;
          prev[next.second][next.first] = current;
          queue.push(next);
        }
      }
    }
  }

  return {std::move(dist), last_point};
}

EnemyTankAI::Path EnemyTankAI::ReconstructPath(
    const std::vector<std::vector<int64_t>>& dist, const Point& start,
    const Point& end) const noexcept {
  if (dist[end.second][end.first] == INT_MAX) {
    return {};
  }

  Path path;
  Point current = end;
  int block_size = m_level->getBlockSize();
  std::random_device rd;
  std::mt19937 gen(rd());

  while (current != start) {
    path.emplace_back(current.first * block_size, current.second * block_size);

    const auto& level = m_level->GetLevelDescription();
    const std::vector<Point> directions = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    std::vector<Point> valid_neighbors;

    // Собираем все валидные соседние точки с меньшим расстоянием
    for (const auto& dir : directions) {
      Point neighbor = {current.first + dir.first, current.second + dir.second};

      if (neighbor.first >= 0 && neighbor.first < level[0].size() &&
          neighbor.second >= 0 && neighbor.second < level.size()) {
        if (dist[neighbor.second][neighbor.first] <
            dist[current.second][current.first]) {
          valid_neighbors.push_back(neighbor);
        }
      }
    }

    // Если нет валидных соседей, прерываем цикл
    if (valid_neighbors.empty()) {
      break;
    }

    // Случайным образом выбираем следующую точку из валидных соседей
    std::uniform_int_distribution<> dis(0, valid_neighbors.size() - 1);
    current = valid_neighbors[dis(gen)];
  }

  // Добавляем стартовую точку, если еще не добавлена
  if (!path.empty() && path.back().first != start.first * block_size &&
      path.back().second != start.second * block_size) {
    path.emplace_back(start.first * block_size, start.second * block_size);
  }

  std::reverse(path.begin(), path.end());
  return path;
}

void EnemyTankAI::Update(double delta) noexcept {
  if (!m_enemy_tank || !m_path_to_eagle ||
      m_current_path_index >= m_path_to_eagle->size()) {
    if (m_enemy_tank) {
      m_enemy_tank->SetVelocity(0);
    }
    return;
  }

  m_enemy_tank->fair();

  float block_size = m_level->getBlockSize();
  float tank_x =
      (m_enemy_tank->GetPosition().x - m_level->getLeftOffset()) / block_size;
  float tank_y = (m_level->GetGameStateHeight() - m_level->getTopOffset() -
                  m_enemy_tank->SetSize().y - m_enemy_tank->GetPosition().y) /
                 block_size;

  const auto& target_point = m_path_to_eagle->at(m_current_path_index);
  float target_x = target_point.first / block_size;
  float target_y = target_point.second / block_size;

  if (std::abs(tank_x - target_x) > std::abs(tank_y - target_y)) {
    m_enemy_tank->SetOrientation(tank_x < target_x
                                     ? EnemyTank::EOrientation::Right
                                     : EnemyTank::EOrientation::Left);
  } else {
    m_enemy_tank->SetOrientation(tank_y < target_y
                                     ? EnemyTank::EOrientation::Bottom
                                     : EnemyTank::EOrientation::Top);
  }

  m_enemy_tank->SetVelocity(m_enemy_tank->GetMaxVelocity());

  float threshold = 0.03f;
  if (std::abs(tank_x - target_x) < threshold &&
      std::abs(tank_y - target_y) < threshold) {
    m_current_path_index++;
  }
}

}  // namespace BatleCity