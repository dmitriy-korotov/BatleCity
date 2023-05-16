#include "EnemyTankAI.h"

#include "../GameObjects/EnemyTank.h"
#include "../GameStates/Level.h"

#include <unordered_set>
#include <deque>



namespace BatleCity
{
	EnemyTankAI::EnemyTankAI(std::shared_ptr<const Level> level_ptr) noexcept
			: m_level(std::move(level_ptr))
	{ }



	void EnemyTankAI::activeOnTank(std::shared_ptr<EnemyTank> enemy_tank) noexcept
	{
		std::swap(m_enemy_tank, enemy_tank);
		m_eagle_position = findEaglePosition();
		m_path_to_eagle = calculatePathToEagle();

		if (m_path_to_eagle.has_value())
		{
			for (const auto& point : *m_path_to_eagle)
			{
				std::cout << '(' << point.first / 16 << ", " << point.second / 16 << ')' << std::endl;
			}
		}
	}



	std::optional<EnemyTankAI::Point> EnemyTankAI::findEaglePosition() const noexcept
	{
		const auto& level_description = m_level->getLevelDescription();
		for (uint16_t i = 0; i < level_description.size(); ++i)
		{
			for (uint16_t j = 0; j < level_description[i].size(); ++j)
			{
				if (level_description[i][j] == EAGLE_SYMBOL)
				{
					return std::make_pair(j, i);
				}
			}
		}
		return std::optional<Point>();
	}



	std::optional<EnemyTankAI::Path> EnemyTankAI::calculatePathToEagle() const noexcept
	{
		if (m_eagle_position.has_value())
		{
			Point _indexes_tank_position = getIndexesTankPosition();

			auto [dp , last_visited_point] = getFilledDynamic(_indexes_tank_position, m_eagle_position);

			return getPathFromDynamic(dp, last_visited_point, _indexes_tank_position);
		}
		return std::optional<Path>();
	}



	EnemyTankAI::Point EnemyTankAI::getIndexesTankPosition() const noexcept
	{
		size_t x_index = static_cast<size_t>(std::round((m_enemy_tank->getPosition().x - m_level->getLeftOffset()) / m_level->getBlockSize()));
		size_t y_index = static_cast<size_t>(std::round(
			(m_level->getGameStateHeight() - m_level->getTopOffset() - m_enemy_tank->getSize().y - m_enemy_tank->getPosition().y) / m_level->getBlockSize()
													   ));
		return { x_index, y_index };
	}



	std::pair<std::vector<std::vector<int64_t>>, EnemyTankAI::Point> EnemyTankAI::getFilledDynamic(const Point& tank_position,
																								   const std::optional<Point>& eagle_position) const noexcept
	{
		const auto& level_description = m_level->getLevelDescription();
		size_t level_height = level_description.size();
		size_t level_width = level_description[0].size();

		std::vector<std::vector<int64_t>> dp(level_height, std::vector<int64_t>(level_width, INT_MAX));
		dp[tank_position.first][tank_position.second] = 0;

		std::deque<Point> _points_witch_need_visit;

		_points_witch_need_visit.emplace_back(tank_position);
		Point _last_visited_point = tank_position;

		while (!_points_witch_need_visit.empty())
		{
			Point _current_point = _points_witch_need_visit.back();
			_points_witch_need_visit.pop_back();

			_last_visited_point = _current_point;

			if (_current_point.first >= 1 && _current_point.second >= 1)
			{
				dp[_current_point.second][_current_point.first] = std::min(
					dp[_current_point.second][_current_point.first - 1],
					dp[_current_point.second - 1][_current_point.first]
				) + 1;
			}
			else if (_current_point.first >= 1)
			{
				dp[_current_point.second][_current_point.first] = dp[_current_point.second][_current_point.first - 1] + 1;
			}
			else if (_current_point.second >= 1)
			{
				dp[_current_point.second][_current_point.first] = dp[_current_point.second - 1][_current_point.first] + 1;
			}

			if (level_description[_current_point.second][_current_point.first] == EAGLE_SYMBOL)
			{
				break;
			}

			if (_current_point.first < eagle_position->first && level_description[_current_point.second][_current_point.first + 1] == PERMITTED_FOR_PATH_SYMBOL)
			{
				_points_witch_need_visit.emplace_front(_current_point.first + 1, _current_point.second);
			}
			if (_current_point.second < eagle_position->second && level_description[_current_point.second + 1][_current_point.first] == PERMITTED_FOR_PATH_SYMBOL)
			{
				_points_witch_need_visit.emplace_front(_current_point.first, _current_point.second + 1);
			}
		}

		return { std::move(dp), std::move(_last_visited_point) };
	}



	EnemyTankAI::Path EnemyTankAI::getPathFromDynamic(std::vector<std::vector<int64_t>>& dp, const Point& last_visited_point, const Point& tank_position) const noexcept
	{
		Path _result;
		for (int64_t x = last_visited_point.first, y = last_visited_point.second; y > tank_position.second || x > tank_position.first;)
		{
			if (x == tank_position.first)
			{
				_result.emplace_back(x * m_level->getBlockSize(), (y--) * m_level->getBlockSize());
			}
			else if (y == tank_position.second)
			{
				_result.emplace_back((x--) * m_level->getBlockSize(), y * m_level->getBlockSize());
			}
			else
			{
				if (dp[y - 1][x] > dp[y][x - 1])
				{
					_result.emplace_back((x--) * m_level->getBlockSize(), y * m_level->getBlockSize());
				}
				else
				{
					_result.emplace_back(x * m_level->getBlockSize(), (y--) * m_level->getBlockSize());
				}
			}
		}
		std::reverse(_result.begin(), _result.end());

		return _result;
	}



	void EnemyTankAI::update(double delta) noexcept
	{
		if (m_enemy_tank && m_path_to_eagle.has_value() && m_current_path_index < m_path_to_eagle->size())
		{
			int64_t _transformed_vertical_position = std::ceil(m_level->getGameStateHeight() - m_enemy_tank->getSize().y - m_enemy_tank->getPosition().y - m_level->getTopOffset());
			int64_t _transformed_horisontal_position = std::ceil(m_enemy_tank->getPosition().x - m_level->getLeftOffset());

			if (_transformed_horisontal_position < m_path_to_eagle->at(m_current_path_index).first)
			{
				m_enemy_tank->setOrientation(EnemyTank::EOrientation::Right);
			}
			else
			{
				if (m_current_path_index + 1 != m_path_to_eagle->size())
				{
					m_enemy_tank->setOrientation(EnemyTank::EOrientation::Bottom);
				}
			}
			m_enemy_tank->setVelocity(m_enemy_tank->getMaxVelocity());

			switch (m_enemy_tank->getOrientation())
			{
			case EnemyTank::EOrientation::Bottom:
				if (_transformed_vertical_position >= m_path_to_eagle->at(m_current_path_index).second)
				{
					++m_current_path_index;
				}
				break;
			case EnemyTank::EOrientation::Right:
				if (_transformed_horisontal_position >= m_path_to_eagle->at(m_current_path_index).first)
				{
					++m_current_path_index;
				}
				break;
			}
		}
		else
		{
			if (m_enemy_tank && m_path_to_eagle.has_value())
			{
				if (m_eagle_position->first * m_level->getBlockSize() == m_path_to_eagle->back().first)
				{
					m_enemy_tank->setOrientation(EnemyTank::EOrientation::Bottom);
				}
				else if (m_eagle_position->second * m_level->getBlockSize() == m_path_to_eagle->back().second)
				{
					m_enemy_tank->setOrientation(EnemyTank::EOrientation::Right);
				}

				m_enemy_tank->setVelocity(0);
				m_enemy_tank->fair();
			}
		}
	}
}