#pragma once
#ifndef GAME_OBJECT_COLLIDER
#define GAME_OBJECT_COLLIDER

#include "../../Physics/AxisAlignedBoundingBox.h"

#include "../../Render/IRenderObjectRect2D.h"

#include <vector>

namespace BatleCity
{
	class GameObjectCollider : RenderEngine::IRenderObjectRect2D
	{
	public:

		GameObjectCollider(const GameObjectCollider&) = delete;
		GameObjectCollider(GameObjectCollider&&) = delete;

		GameObjectCollider(std::shared_ptr<RenderEngine::ShaderProgram> shader_program, const glm::vec3& color = glm::vec3(1.f, 0.f, 0.f));

		void addCollider(const glm::vec2& bottom_left, const glm::vec2& right_top);
		void deleteAllColliders();
		inline const std::vector<Physics::AABB>& getColliders() const noexcept { return m_rect_colliders; }

		void render(const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer) const override;

		void swap(std::vector<Physics::AABB>& rect_colliders);

		~GameObjectCollider() override;

	private:

		glm::vec3 m_color = glm::vec3(1.f, 0.f, 0.f);
		std::vector<Physics::AABB> m_rect_colliders;

		RenderEngine::VertexBuffer m_vertex_colors_buffer;

	};
}

#endif // !GAME_OBJECT_COLLIDER
