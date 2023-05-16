#pragma once
#ifndef ENEMY_TANK_H
#define ENEMY_TANK_H

#include "Tank.h"
#include "../GameAI/EnemyTankAI.h"



namespace BatleCity
{
	class EnemyTank : public Tank, public std::enable_shared_from_this<EnemyTank>
	{
	public:

		EnemyTank(std::shared_ptr<const Level> level_ptr, ETankType tank_type, const glm::vec2& position, const glm::vec2& size,
				  double max_velocity, double delay_between_shots = 200, const glm::vec2& direction = glm::vec2(0.f, 1.f),
				  double velocity = 0, float layer = 0.f);

		void active() noexcept;
		void update(double delta) override;

	private:

		EnemyTankAI m_AI;

	};
}

#endif // !ENEMY_TANK_H
