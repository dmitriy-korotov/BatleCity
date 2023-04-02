#include "IGameObject.h"

namespace BatleCity
{
	bool IGameObject::m_is_rendering_colliders = false;



	void IGameObject::renderColliders() const
	{
		if (m_is_rendering_colliders)
		{
			m_colliders.render(m_position, m_size, m_rotation, 1.f);
		}
	}
}