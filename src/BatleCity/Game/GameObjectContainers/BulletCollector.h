#pragma once
#ifndef BULLET_COLLECTOR_H
#define BULLET_COLLECTOR_H

#include <unordered_set>

#include "../GameObjects/Bullet.h"

namespace BatleCity {
class BulletCollector {
 public:
  BulletCollector() = default;
  BulletCollector(const BulletCollector&) = default;
  BulletCollector(BulletCollector&&) = default;
  ~BulletCollector() = default;

  bool Has(std::size_t id) const;

  void AddBullet(std::shared_ptr<Bullet> bullet);

  void UpdateBullets(double delta);
  void RenderBullets() const;

 private:
  std::unordered_set<std::shared_ptr<Bullet>> m_bullets;
};
}  // namespace BatleCity

#endif  // !BULLET_COLLECTOR_H
