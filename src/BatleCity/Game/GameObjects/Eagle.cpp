#include "Eagle.h"

#include "../../Resources/ResourceManager.h"

namespace BatleCity {
Eagle::Eagle(const glm::vec2& position, const glm::vec2& size,
             const float rotation, const float layer)
    : IGameObject(EGameObjectType::Eagle, position, size, rotation, layer),
      m_eagle_state(EEagleState::Elive),
      m_eagle_sprites{
          Game::Instance().GetResourcesManager()->GetSprite("Eagle_Elive"),
          Game::Instance().GetResourcesManager()->GetSprite("Eagle_Dead")} {
  m_colliders.AddCollider(glm::vec2(0.f), m_size);
}

void Eagle::Render() const {
  m_eagle_sprites[static_cast<size_t>(m_eagle_state)]->Render(
      m_position, m_size, m_rotation, m_layer);
}

void Eagle::Update(const double delta) {}

bool Eagle::OnCollision(EGameObjectType game_object_type,
                        std::shared_ptr<IGameObject> object,
                        std::shared_ptr<Physics::AABB> target_collider,
                        const glm::vec2& direction) {
  if (game_object_type == EGameObjectType::Bullet) {
    m_eagle_state = EEagleState::Dead;
    m_colliders.DeleteAllColliders();
  }
  return true;
}
}  // namespace BatleCity