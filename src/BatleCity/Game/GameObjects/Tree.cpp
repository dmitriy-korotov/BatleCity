#include "Tree.h"

#include "../../Resources/ResourceManager.h"

namespace BatleCity {
Tree::Tree(const EBlockType tree_type, const glm::vec2& position,
           const glm::vec2& size, const float rotation, const float layer)
    : Block(EGameObjectType::Tree, tree_type, position, size, rotation, layer),
      m_sprite(Game::Instance().GetResourcesManager()->GetSprite("Tree_8x8")) {
  m_colliders.AddCollider(glm::vec2(0.f), m_size);
}

void Tree::RenderBlock(const ESubBlockLocation subBlock_location) const {
  switch (subBlock_location) {
    case ESubBlockLocation::LeftTop:
      m_sprite->Render(m_position + glm::vec2(0.f, m_size.y / 2), m_size / 2.f,
                       m_rotation, m_layer);
      break;
    case ESubBlockLocation::RightBottom:
      m_sprite->Render(m_position + glm::vec2(m_size.x / 2, 0.f), m_size / 2.f,
                       m_rotation, m_layer);
      break;
    case ESubBlockLocation::RightTop:
      m_sprite->Render(m_position + m_size / 2.f, m_size / 2.f, m_rotation,
                       m_layer);
      break;
    case ESubBlockLocation::LeftBottom:
      m_sprite->Render(m_position, m_size / 2.f, m_rotation, m_layer);
      break;
    default:
      break;
  }
}
}  // namespace BatleCity