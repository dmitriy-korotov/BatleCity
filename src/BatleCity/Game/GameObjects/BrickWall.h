#pragma once
#ifndef BRICK_WALL_H
#define BRICK_WALL_H

#include <memory>
#include <vector>

#include "Block.h"

#include "../../Render/Sprite2D.h"

namespace BatleCity {
class BrickWall : public Block {
 public:
  BrickWall(const EBlockType wall_type, const glm::vec2& position,
            const glm::vec2& size, const float rotation, const float layer);

  void RenderBlock(const ESubBlockLocation subBlock_location) const override;
  bool OnCollision(EGameObjectType game_object_type,
                   std::shared_ptr<IGameObject> object,
                   std::shared_ptr<Physics::AABB> target_collider,
                   const glm::vec2& direction) override;

 private:
  glm::vec2 GetOffsetBySubBlockLocation(
      ESubBlockLocation subBlockLocation) const noexcept;
  ESubBlockLocation GetSubBlockLocationByCollider(
      std::shared_ptr<Physics::AABB>& collider) const noexcept;
  size_t GetColliderIndexBySubBlockLocation(
      ESubBlockLocation subBlock_location) const noexcept;
  void ChangeBrickWallAfterCollisionWithBullet(
      ESubBlockLocation& subBlock_location, const glm::vec2& direction);

  std::shared_ptr<RenderEngine::Sprite2D> m_left_bottom_sprite = nullptr;
  std::shared_ptr<RenderEngine::Sprite2D> m_left_top_sprite = nullptr;
  std::shared_ptr<RenderEngine::Sprite2D> m_right_bottom_sprite = nullptr;
  std::shared_ptr<RenderEngine::Sprite2D> m_right_top_sprite = nullptr;

  std::vector<EBlockType> m_subBlock_types = {EBlockType::All, EBlockType::All,
                                              EBlockType::All, EBlockType::All};
};
}  // namespace BatleCity

#endif  // !BRICK_WALL_H
