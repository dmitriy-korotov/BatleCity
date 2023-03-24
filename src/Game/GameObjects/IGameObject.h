#pragma once
#ifndef I_GAME_OBJECT_H
#define I_GAME_OBJECT_H

#include <vector>

#include <glm/vec2.hpp>

#include "GameObjectCollider.h"

#include "../../Resources/ResourceManager.h"

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
		IGameObject(EGameObjectType game_object_type, const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer)
			: m_game_object_type(game_object_type)
			, m_colliders(Resources::ResourceManager::getShaderProgram("ColliderShaderProgram"))
			, m_size(size)
			, m_position(position)
			, m_rotation(rotation)
			, m_layer(layer)
		{ }

		virtual ~IGameObject()
		{ }

		virtual void update(const double daleta) = 0;
		virtual void render() const = 0;

		void renderColliders() const;

		inline void setPosition(const glm::vec2& positiion) { m_position = positiion; }
		inline void setSize(const glm::vec2& size) { m_size = size; }
		inline void setRotation(const float rotation) { m_rotation = rotation; }
		inline void setLayer(const float layer) { m_layer = layer; }

		inline const std::vector<Physics::AABB>& getColliders() const noexcept { return m_colliders.getColliders(); }
		inline const glm::vec2& getPosition() const noexcept { return m_position; }
		inline const glm::vec2& getSize() const noexcept { return m_size; }
		inline float getRotation() const noexcept { return m_rotation; }
		inline EGameObjectType getGameObjectType() const noexcept { return m_game_object_type; }

	protected:

		EGameObjectType m_game_object_type;
		GameObjectCollider m_colliders;
		glm::vec2 m_size = glm::vec2(0.f);
		glm::vec2 m_position = glm::vec2(0.f);
		float m_rotation = 0.f;
		float m_layer = 0.f;

	};
}

#endif // !I_GAME_OBJECT_H
