#include "BulletCollector.h"

#include <vector>

namespace BatleCity {
bool BulletCollector::Has(std::size_t id) const {
  for (const auto& bullet : m_bullets) {
    if (bullet->GetID() == id) {
      return true;
    }
  }
  return false;
}

void BulletCollector::AddBullet(std::shared_ptr<Bullet> bullet) {
  m_bullets.insert(std::move(bullet));
}

void BulletCollector::UpdateBullets(double delta) {
  if (!m_bullets.empty()) {
    std::vector<std::shared_ptr<Bullet>> deleted_bullets;
    deleted_bullets.reserve(m_bullets.size());

    for (const auto& bullet : m_bullets) {
      if (bullet->IsDestroy()) {
        deleted_bullets.emplace_back(bullet);
      } else {
        bullet->Update(delta);
      }
    }
    for (const auto& bullet : deleted_bullets) {
      m_bullets.erase(bullet);
    }
  }
}

void BulletCollector::RenderBullets() const {
  for (const auto& bullet : m_bullets) {
    bullet->Render();
    bullet->RenderColliders();
  }
}
}  // namespace BatleCity