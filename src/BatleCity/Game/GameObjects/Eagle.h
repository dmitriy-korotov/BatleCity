#pragma once
#ifndef EAGLE_H
#define EAGLE_H

#include <array>
#include <memory>

#include "IGameObject.h"

#include "../../Render/Sprite2D.h"

namespace BatleCity {
class Eagle : public IGameObject {
 public:
  enum class EEagleState { Elive = 0, Dead };

  Eagle(const glm::vec2& positiion, const glm::vec2& size, const float rotation,
        const float layer);

  EEagleState GetState() const noexcept { return m_eagle_state; }

  void Render() const final;
  void Update(const double delta) final;
  bool OnCollision(EGameObjectType game_object_type,
                   std::shared_ptr<IGameObject> object,
                   std::shared_ptr<Physics::AABB> target_collider,
                   const glm::vec2& direction = glm::vec2(0.f)) final;

 private:
  EEagleState m_eagle_state;
  const std::array<std::shared_ptr<RenderEngine::Sprite2D>, 2> m_eagle_sprites;
};
}  // namespace BatleCity

#endif  // !EAGLE_H
