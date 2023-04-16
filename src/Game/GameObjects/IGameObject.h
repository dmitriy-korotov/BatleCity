#pragma once
#ifndef I_GAME_OBJECT_H
#define I_GAME_OBJECT_H

#include <glm/vec2.hpp>

#include "GameObjectCollider.h"

#include "../../Resources/ResourceManager.h"

#include <vector>



namespace RenderEngine
{
	class Sprite2D;
}

namespace BatleCity
{
	class IGameObject
	{
	public:


		enum class EGameObjectType : uint8_t
		{
			BrickWall,
			BetonWall,
			Border,
			Tank,
			Tree,
			Water,
			Bullet,
			Ice,
			Eagle
		};


		IGameObject() = default;
		IGameObject(EGameObjectType game_object_type, const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer);

		virtual ~IGameObject()
		{ }

		virtual bool onCollision(EGameObjectType game_object_type, std::shared_ptr<Physics::AABB> target_collider, const glm::vec2& direction = glm::vec2(0.f)) = 0;
		virtual void update(const double daleta) = 0;
		virtual void render() const = 0;

		void renderColliders() const;

		inline void setPosition(const glm::vec2& positiion) noexcept { m_position = positiion; }
		inline void setSize(const glm::vec2& size) noexcept { m_size = size; }
		inline void setRotation(const float rotation) noexcept { m_rotation = rotation; }
		inline void setLayer(const float layer) noexcept { m_layer = layer; }

		inline const std::vector<Physics::AABB>& getColliders() const noexcept { return m_colliders.getColliders(); }
		inline const glm::vec2& getPosition() const noexcept { return m_position; }
		inline const glm::vec2& getSize() const noexcept { return m_size; }
		inline float getRotation() const noexcept { return m_rotation; }
		inline EGameObjectType getGameObjectType() const noexcept { return m_game_object_type; }

		inline static void enableRenderingColliders() noexcept { m_is_rendering_colliders = true; }
		inline static void disableRenderingColliders() noexcept { m_is_rendering_colliders = false; }

	protected:

		static bool m_is_rendering_colliders;

		EGameObjectType m_game_object_type;

		GameObjectCollider m_colliders;

		glm::vec2 m_size = glm::vec2(0.f);
		glm::vec2 m_position = glm::vec2(0.f);
		float m_rotation = 0.f;
		float m_layer = 0.f;

	};
}

#endif // !I_GAME_OBJECT_H
