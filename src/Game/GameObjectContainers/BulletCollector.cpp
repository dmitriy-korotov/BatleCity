#include "BulletCollector.h"

#include <vector>

namespace BatleCity
{
	void BulletCollector::addBullet(std::shared_ptr<Bullet> bullet)
	{
		m_bullets.insert(std::move(bullet));
	}



	void BulletCollector::updateBullets(double delta)
	{
		if (!m_bullets.empty())
		{
			std::vector<std::shared_ptr<Bullet>> deleted_bullets;
			deleted_bullets.reserve(m_bullets.size());

			for (const auto& bullet : m_bullets)
			{
				if (bullet->isDestroy())
				{
					deleted_bullets.emplace_back(bullet);
				}
				else
				{
					bullet->update(delta);
				}
			}
			for (const auto& bullet : deleted_bullets)
			{
				m_bullets.erase(bullet);
			}
		}
	}



	void BulletCollector::renderBullets() const
	{
		for (const auto& bullet : m_bullets)
		{
			bullet->render();
			bullet->renderColliders();
		}
	}
}