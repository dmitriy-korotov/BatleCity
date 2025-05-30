#include "Border.h"

#include "../../Resources/ResourceManager.h"

namespace BatleCity {
Border::Border(const glm::vec2& position, const glm::vec2& size,
               const float rotation, const float layer)
    : Block(EGameObjectType::Border, EBlockType::All, position, size, rotation,
            layer),
      m_sprite(
          Game::Instance().GetResourcesManager()->GetSprite("Border_8x8")) {
  m_colliders.AddCollider(glm::vec2(0.f), m_size);
}

void Border::Render() const {
  m_sprite->Render(m_position, m_size, m_rotation, m_layer);
}
}  // namespace BatleCity
