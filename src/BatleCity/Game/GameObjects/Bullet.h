#pragma once
#ifndef BULLET_H
#define BULLET_H

#include <memory>

#include "IDynamicGameObject.h"

#include "../../Render/SpriteAnimator.h"

#include "../../System/Timer.h"

namespace BatleCity {
class Bullet : public IDynamicGameObject {
 public:
  Bullet(std::size_t owner_id, EOrientation bullet_type, const glm::vec2& size, float layer,
         double max_velocity);

  void fire(const glm::vec2& position, const glm::vec2& direction,
            double velocity);

  void update(double delta) override;
  void render() const override;

  bool onCollision(EGameObjectType game_object_type,
                   std::shared_ptr<IGameObject> object,
                   std::shared_ptr<Physics::AABB> target_collider,
                   const glm::vec2& direction = glm::vec2(0.f)) override;

 private:
  std::size_t m_owner_id = 0;
  std::shared_ptr<RenderEngine::Sprite2D> m_sprite = nullptr;
  std::pair<RenderEngine::SpriteAnimator, System::Timer>
      m_explosion_animation;
  bool m_is_fire = false;
  bool m_is_exploded = false;
};
}  // namespace BatleCity

#endif  // !BULLET_H
