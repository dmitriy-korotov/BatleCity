#pragma once
#ifndef I_RENDER_OBJECT_RECT_2D
#define I_RENDER_OBJECT_RECT_2D

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "ElementBuffer.h"
#include "ShaderProgram.h"

#include <memory>

namespace RenderEngine
{
	class IRenderObjectRect2D
	{
	public:

		IRenderObjectRect2D(std::shared_ptr<ShaderProgram> shader_program);

		virtual void render(const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer) const = 0;
		virtual ~IRenderObjectRect2D();

	protected:

		std::shared_ptr<ShaderProgram> m_shader_program = nullptr;

		VertexArray m_VAO;
		VertexBuffer m_vertex_coords_buffer;
		ElementBuffer m_EBO;

	};
}

#endif // !I_RENDER_OBJECT_RECT_2D
