#include "IDynamicGameObject.h"



namespace BatleCity
{
	IDynamicGameObject::IDynamicGameObject(EGameObjectType game_object_type, const glm::vec2& position, const glm::vec2& size,
										   float rotation, float layer, const glm::vec2& direction, double velocity, double max_velocity)

				: IGameObject(game_object_type, position, size, rotation, layer)
				, m_direction(direction)
				, m_velocity(velocity)
				, m_max_velocity(max_velocity)
	{ }



	void IDynamicGameObject::move(const glm::vec2& offset)
	{
		m_position += offset;
	}



	void IDynamicGameObject::setVelocity(double velocity)
	{
		m_velocity = velocity;
	}



	void IDynamicGameObject::setDirection(const glm::vec2& direction)
	{
		m_direction = direction;
	}



	void IDynamicGameObject::setOrientation(const EOrientation orientation)
	{
		m_current_orientation = orientation;
	}



	double IDynamicGameObject::getMaxVelocity() const
	{
		return m_max_velocity;
	}



	double IDynamicGameObject::getVelocity() const
	{
		return m_velocity;
	}



	const glm::vec2& IDynamicGameObject::getDirection() const
	{
		return m_direction;
	}



	IDynamicGameObject::EOrientation IDynamicGameObject::getOrientation() const noexcept
	{
		return m_current_orientation;
	}
}