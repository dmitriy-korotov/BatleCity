#include "IDynamicGameObject.h"

namespace BatleCity
{
	IDynamicGameObject::IDynamicGameObject(const glm::vec2& position, const glm::vec2& size, float rotation,
										 float layer, const glm::vec2& direction, double velocity)
			: IGameObject(position, size, rotation, layer)
			, m_direction(direction)
			, m_velocity(velocity)
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



	double IDynamicGameObject::getVelocity() const
	{
		return m_velocity;
	}



	const glm::vec2& IDynamicGameObject::getDirection() const
	{
		return m_direction;
	}
}