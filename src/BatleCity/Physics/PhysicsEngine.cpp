#include "PhysicsEngine.h"

#include <memory>

namespace Physics {
void PhysicsEngine::Init() {}

void PhysicsEngine::RemoveAllDynamicObjects() { m_dynamicGameObjects.clear(); }

void PhysicsEngine::Terminate() {
  m_dynamicGameObjects.clear();
  m_currentLevel.reset();
}

void PhysicsEngine::Update(double delta) {
  assert(m_currentLevel);

  auto it = std::begin(m_dynamicGameObjects);
  while (it != std::end(m_dynamicGameObjects)) {
    const auto id = it->first;
    const auto& object = it->second;

    if (object->IsDestroy()) {
      it = m_dynamicGameObjects.erase(it);
      continue;
    }
    if (object->GetVelocity() <= 0) {
      it = std::next(it);
      continue;
    }

    const auto newPosition = GetNewPosition(object, delta);

    const auto mapObjects =
        m_currentLevel->GetObjectsFromArea(newPosition, object->gSetSize());
    auto isIntersection =
        IsInersectionWithObjects(object, newPosition, mapObjects);

    for (const auto& [otherObjectID, otherObject] : m_dynamicGameObjects) {
      if (id == otherObjectID) {
        continue;
      }
      const auto [first, second] = IsIntersection(
          object->getColliders(), newPosition, otherObject->getColliders(),
          otherObject->getPosition());
      if (first || second) {
        isIntersection |= object->onCollision(otherObject->getGameObjectType(),
                                              otherObject, second);
        otherObject->onCollision(object->getGameObjectType(), object, first);
      }
    }

    if (!isIntersection) {
      object->setPosition(newPosition);
    }
    it = std::next(it);
  }
}

void PhysicsEngine::SetCurrentLevel(
    std::shared_ptr<const BatleCity::Level> current_level) {
  m_currentLevel.swap(current_level);
}

void PhysicsEngine::AddDynamicGameObject(
    std::shared_ptr<BatleCity::IDynamicGameObject> object) {
  m_dynamicGameObjects.emplace(object->GetID(), std::move(object));
}

glm::vec2 PhysicsEngine::GetNewPosition(
    const std::shared_ptr<BatleCity::IDynamicGameObject>& game_object,
    double delta) {
  const glm::vec2 objectDirection = game_object->getDirection();
  glm::vec2 newPosition(0.f);

  if (objectDirection.x == 0.f) {
    newPosition.x = static_cast<float>(
        static_cast<int>(game_object->getPosition().x / 4.f + 0.5f) * 4);
    newPosition.y = game_object->getPosition().y +
                    objectDirection.y *
                        static_cast<float>(game_object->GetVelocity() * delta);
  } else {
    newPosition.y = static_cast<float>(
        static_cast<int>(game_object->getPosition().y / 4.f + 0.5f) * 4);
    newPosition.x = game_object->getPosition().x +
                    objectDirection.x *
                        static_cast<float>(game_object->GetVelocity() * delta);
  }
  return newPosition;
}

std::pair<std::shared_ptr<AABB>, std::shared_ptr<AABB>>
PhysicsEngine::IsIntersection(const std::vector<AABB>& first_object,
                              const glm::vec2& position_object1,
                              const std::vector<AABB>& second_object,
                              const glm::vec2& position_object2) {
  for (const auto& first_collision : first_object) {
    for (const auto& second_collision : second_object) {
      if (first_collision.isActive() && second_collision.isActive()) {
        if (first_collision.getLeftBottom().x + position_object1.x >=
            second_collision.getRightTop().x + position_object2.x) {
          continue;
        }
        if (first_collision.getRightTop().x + position_object1.x <=
            second_collision.getLeftBottom().x + position_object2.x) {
          continue;
        }
        if (first_collision.getLeftBottom().y + position_object1.y >=
            second_collision.getRightTop().y + position_object2.y) {
          continue;
        }
        if (first_collision.getRightTop().y + position_object1.y <=
            second_collision.getLeftBottom().y + position_object2.y) {
          continue;
        }
        return std::make_pair<std::shared_ptr<AABB>, std::shared_ptr<AABB>>(
            std::make_shared<AABB>(first_collision),
            std::make_shared<AABB>(second_collision));
      }
    }
  }
  return std::make_pair<std::shared_ptr<AABB>, std::shared_ptr<AABB>>(nullptr,
                                                                      nullptr);
}

bool PhysicsEngine::IsInersectionWithObjects(
    const std::shared_ptr<BatleCity::IDynamicGameObject>& current_game_object,
    const glm::vec2& newPosition,
    const std::vector<std::shared_ptr<BatleCity::IGameObject>>& other_objects) {
  for (const auto& object : other_objects) {
    auto colliders_pair =
        IsIntersection(current_game_object->getColliders(), newPosition,
                       object->getColliders(), object->getPosition());
    if (colliders_pair.first != nullptr && colliders_pair.second != nullptr) {
      const bool is_stoped = current_game_object->onCollision(
          object->getGameObjectType(), object, colliders_pair.first);
      object->onCollision(current_game_object->getGameObjectType(),
                          current_game_object, colliders_pair.second,
                          current_game_object->getDirection());
      if (is_stoped) return true;
    }
  }
  return false;
}
}  // namespace Physics