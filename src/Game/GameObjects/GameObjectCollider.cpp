#include "GameObjectCollider.h"

#include "../../Render/VertexBufferLayout.h"
#include "../../Render/Renderer.h"

#include <glm/gtc/matrix_transform.hpp>

namespace BatleCity
{
	GameObjectCollider::GameObjectCollider(std::shared_ptr<RenderEngine::ShaderProgram> shader_program, const glm::vec3& color)
			: IRenderObjectRect2D(std::move(shader_program))
		    , m_color(color)
	{
		GLfloat colors[] = {
				color.r,	color.g,	color.b,
				color.r,	color.g,	color.b,
				color.r,	color.g,	color.b,
				color.r,	color.g,	color.b
		};

		GLuint indexes[] = {
			0, 1, 2, 3, 0
		};



		m_VAO.bind();

		m_vertex_colors_buffer.init(colors, 3 * 4 * sizeof(GLfloat));
		RenderEngine::VertexBufferLayout vertex_colors_layout;
		vertex_colors_layout.addBufferLayoutElementFloat(3, GL_FALSE);
		m_VAO.addLayoutBuffer(m_vertex_colors_buffer, vertex_colors_layout);

		m_EBO.init(indexes, 5);
	}



	void GameObjectCollider::addCollider(const glm::vec2& bottom_left, const glm::vec2& right_top)
	{
		m_rect_colliders.emplace_back(bottom_left, right_top);
	}



	void GameObjectCollider::deleteAllColliders()
	{
		m_rect_colliders.clear();
	}



	void GameObjectCollider::render(const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer) const
	{
		glm::mat4 model_matrix(1.f);
		m_shader_program->use();
		m_shader_program->setFloat("layer", layer);

		for (const auto& rect_collider : m_rect_colliders)
		{
			if (rect_collider.isActive())
			{
				model_matrix = glm::translate(model_matrix, glm::vec3(position + rect_collider.getLeftBottom(), 0.f));
				model_matrix = glm::translate(model_matrix, glm::vec3(0.5f * (rect_collider.getRightTop() - rect_collider.getLeftBottom()), 0.f));
				model_matrix = glm::rotate(model_matrix, glm::radians(rotation), glm::vec3(0.f, 0.f, 1.f));
				model_matrix = glm::translate(model_matrix, glm::vec3(-0.5f * (rect_collider.getRightTop() - rect_collider.getLeftBottom()), 0.f));
				model_matrix = glm::scale(model_matrix, glm::vec3(rect_collider.getRightTop() - rect_collider.getLeftBottom(), 1.f));

				m_shader_program->setMatrix4("model_matrix", model_matrix);

				RenderEngine::Renderer::drawElements(GL_LINE_STRIP, m_VAO, m_EBO, *m_shader_program);
			}
		}
	}



	void GameObjectCollider::swap(std::vector<Physics::AABB>& rect_colliders)
	{
		std::swap(m_rect_colliders, rect_colliders);
	}



	Physics::AABB& GameObjectCollider::operator[](size_t index) noexcept
	{
		return m_rect_colliders[index];
	}



	GameObjectCollider::~GameObjectCollider()
	{ }
}