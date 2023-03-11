#pragma once
#ifndef I_GAME_OBJECT_H
#define I_GAME_OBJECT_H

#include <glm/vec2.hpp>

namespace RenderEngine
{
	class Sprite2D;
}

namespace BatleCity
{
	class IGameObject
	{
	public:

		IGameObject() = default;
		IGameObject(const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer)
			: m_size(size), m_position(position), m_rotation(rotation), m_layer(layer)
		{ }

		virtual ~IGameObject()
		{ }

		virtual void update(const uint64_t daleta) = 0;
		virtual void render() const = 0;

		inline void setPosition(const glm::vec2& positiion) { m_position = positiion; }
		inline void setSize(const glm::vec2& size) { m_size = size; }
		inline void setRotation(const float rotation) { m_rotation = rotation; }
		inline void setLayer(const float layer) { m_layer = layer; }

	protected:

		glm::vec2 m_size;
		glm::vec2 m_position;
		float m_rotation;
		float m_layer;

	};
}

#endif // !I_GAME_OBJECT_H
