#include "Tank.h"

#include "Bullet.h"

#include <BatleCity/Game/Game.h>

#include "../../Render/Sprite2D.h"

#include "../../Resources/ResourceManager.h"

#include "../../Physics/PhysicsEngine.h"

namespace BatleCity {
Tank::Tank(ETankType tank_type, const glm::vec2& positiion,
           const glm::vec2& size, const double max_velocity,
           double delay_between_shots, const glm::vec2& direction,
           double velocity, const float layer)

    : IDynamicGameObject(EGameObjectType::Tank, positiion, size, 0.f, layer,
                         direction, velocity, max_velocity),
      m_tank_sprite(Game::Instance().GetResourcesManager()->GetSprite(
          m_all_kinds_tank_sprites[static_cast<size_t>(tank_type)])),
      m_respawn_animation{RenderEngine::SpriteAnimator(
                              Game::Instance().GetResourcesManager()->GetSprite(
                                  "RespawnAnimation")),
                          System::Timer()},
      m_shield_animation{RenderEngine::SpriteAnimator(
                             Game::Instance().GetResourcesManager()->GetSprite(
                                 "ShieldAnimation")),
                         System::Timer()},
      m_min_velocity(max_velocity / 2),
      m_delay_between_shots(delay_between_shots) {
  setPosition(m_position);

  m_respawn_animation.first.setState("default");
  m_respawn_animation.second.SetCallBack([&]() {
    m_is_respawn = false;
    m_has_shild = true;
    m_shield_animation.second.Start(3000);
  });
  m_respawn_animation.second.Start(1500);

  m_shield_animation.first.setState("default");
  m_shield_animation.second.SetCallBack([&]() { m_has_shild = false; });

  m_timer_for_shots.SetCallBack([&]() { m_is_fair = false; });

  m_colliders.addCollider(glm::vec2(0.f), m_size);
}

void Tank::setOrientation(const EOrientation orietation) {
  if (!m_is_respawn) {
    if (m_current_orientation == orietation) {
      return;
    }

    m_current_orientation = orietation;

    switch (m_current_orientation) {
      case EOrientation::Top:
        m_tank_sprite.setState("tankTopState");
        m_direction.x = 0.f;
        m_direction.y = 1.f;
        break;
      case EOrientation::Right:
        m_tank_sprite.setState("tankRightState");
        m_direction.x = 1.f;
        m_direction.y = 0.f;
        break;
      case EOrientation::Bottom:
        m_tank_sprite.setState("tankBottomState");
        m_direction.x = 0.f;
        m_direction.y = -1.f;
        break;
      case EOrientation::Left:
        m_tank_sprite.setState("tankLeftState");
        m_direction.x = -1.f;
        m_direction.y = 0.f;
        break;
      default:
        break;
    }
  }
}

void Tank::setVelocity(double velocity) {
  if (!m_is_respawn) m_velocity = velocity;
}

void Tank::update(const double delta) {
  if (m_is_fair) {
    m_timer_for_shots.Update(delta);
  }
  if (m_is_respawn) {
    m_respawn_animation.first.update(delta);
    m_respawn_animation.second.Update(delta);
  } else {
    if (m_velocity > 0) {
      m_tank_sprite.update(delta);
    }
    if (m_has_shild) {
      m_shield_animation.first.update(delta);
      m_shield_animation.second.Update(delta);
    }
    m_bullets.updateBullets(delta);
  }
}

void Tank::render() const {
  if (m_is_respawn) {
    m_respawn_animation.first.render(m_position, m_size, m_rotation, m_layer);
  } else {
    if (!m_is_destroy) {
      if (m_has_shild) {
        m_shield_animation.first.render(m_position, m_size, m_rotation,
                                        m_layer);
      }
      m_tank_sprite.render(m_position, m_size, m_rotation, m_layer);
    }
    m_bullets.renderBullets();
  }
}

void Tank::fair() const {
  if (!m_is_destroy && !m_is_respawn && !m_is_fair) {
    m_is_fair = true;
    auto bullet =
        std::make_shared<Bullet>(getID(), m_current_orientation, m_size / 2.f,
                                 m_layer + 0.1f, 3 * m_max_velocity);
    bullet->fire(m_position, m_direction, bullet->getMaxVelocity());
    m_bullets.addBullet(bullet);
    Physics::PhysicsEngine::addDynamicGameObject(std::move(bullet));

    m_timer_for_shots.Start(m_delay_between_shots);
  }
}

bool Tank::onCollision(EGameObjectType game_object_type,
                       std::shared_ptr<IGameObject> object,
                       std::shared_ptr<Physics::AABB> target_collider,
                       const glm::vec2& direction) {
  if (game_object_type == EGameObjectType::Tree ||
      game_object_type == EGameObjectType::Ice) {
    return false;
  } else if (game_object_type == EGameObjectType::Bullet) {
    if (!m_bullets.has(object->getID())) {
      if (HP == 1) {
        m_is_destroy = true;
        m_colliders.deleteAllColliders();
      } else {
        HP -= 1;
      }
    }
  }
  return true;
}
}  // namespace BatleCity