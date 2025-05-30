#pragma once
#ifndef PHYSICS_ENGINE_H
#define PHYSICS_ENGINE_H

#include <memory>
#include <unordered_map>
#include <vector>

#include "../Game/GameObjects/IDynamicGameObject.h"
#include "../Game/GameObjects/IGameObject.h"
#include "../Game/GameStates/Level.h"

#include "AxisAlignedBoundingBox.h"

namespace Physics {
class PhysicsEngine final {
 public:
  void Init();
  void Terminate();
  void Update(double delta);
  void RemoveAllDynamicObjects();
  void AddDynamicGameObject(
      std::shared_ptr<BatleCity::IDynamicGameObject> object);
  void SetCurrentLevel(std::shared_ptr<const BatleCity::Level> current_level);

 private:
  glm::vec2 GetNewPosition(
      const std::shared_ptr<BatleCity::IDynamicGameObject>& game_object,
      double delta);

  std::pair<std::shared_ptr<AABB>, std::shared_ptr<AABB>> IsIntersection(
      const std::vector<AABB>& first_object, const glm::vec2& position_object1,
      const std::vector<AABB>& second_object,
      const glm::vec2& position_object2);

  bool IsInersectionWithObjects(
      const std::shared_ptr<BatleCity::IDynamicGameObject>& current_game_object,
      const glm::vec2& new_position,
      const std::vector<std::shared_ptr<BatleCity::IGameObject>>&
          other_objects);

  std::unordered_map<std::size_t,
                     std::shared_ptr<BatleCity::IDynamicGameObject>>
      m_dynamicGameObjects;
  std::shared_ptr<const BatleCity::Level> m_currentLevel;
};
}  // namespace Physics

#endif  // PHYSICS_ENGINE_H
