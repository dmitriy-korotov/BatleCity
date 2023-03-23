#pragma once
#ifndef BULLET_COLLECTOR_H
#define BULLET_COLLECTOR_H

#include <unordered_set>

#include "../GameObjects/Bullet.h"

namespace BatleCity
{
	class BulletCollector
	{
	public:

		BulletCollector() = default;
		BulletCollector(const BulletCollector&) = default;
		BulletCollector(BulletCollector&&) = default;
		~BulletCollector() = default;

		void addBullet(std::shared_ptr<Bullet> bullet);

		void updateBullets(double delta);
		void renderBullets() const;

	private:

		std::unordered_set<std::shared_ptr<Bullet>> m_bullets;

	};
}

#endif // !BULLET_COLLECTOR_H
