#include "EnemyTank.h"



namespace BatleCity
{
	EnemyTank::EnemyTank(std::shared_ptr<const Level> level_ptr, ETankType tank_type, const glm::vec2& position, const glm::vec2& size, double max_velocity,
					     double delay_between_shots, const glm::vec2& direction, double velocity, float layer)
			: Tank(tank_type, position, size, max_velocity, delay_between_shots, direction, velocity, layer)
			, m_AI(std::move(level_ptr))
	{
		setOrientation(EOrientation::Bottom);
	}



	void EnemyTank::active() noexcept
	{
		try
		{
			m_AI.activeOnTank(shared_from_this());
		}
		catch (const std::exception& ex)
		{
			std::cerr << "ERROR: shared_from_this exception (Tank):" << std::endl;
			std::cerr << ex.what() << std::endl;
		}
	}



	void EnemyTank::update(double delta)
	{
		Tank::update(delta);
		m_AI.update(delta);
	}
}