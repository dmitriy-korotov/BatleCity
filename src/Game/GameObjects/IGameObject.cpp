#include "IGameObject.h"

namespace BatleCity
{
	void IGameObject::renderColliders() const
	{
		m_colliders.render(m_position, m_size, m_rotation, 1.f);
	}
}