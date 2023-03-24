#include "IRenderObjectRect2D.h"

#include "VertexBufferLayout.h"

namespace RenderEngine
{
	IRenderObjectRect2D::IRenderObjectRect2D(std::shared_ptr<ShaderProgram> shader_program)
		: m_shader_program(std::move(shader_program))
	{
		GLfloat vertex_coords[] = {
			0.f, 0.f,
			0.f, 1.f,
			1.f, 1.f,
			1.f, 0.f
		};

		m_VAO.bind();
		//					2 - amount coords in one point;		4 - amount points
		m_vertex_coords_buffer.init(vertex_coords, 2 * 4 * sizeof(GLfloat));
		VertexBufferLayout vertex_coords_layout;
		vertex_coords_layout.addBufferLayoutElementFloat(2, false);
		m_VAO.addLayoutBuffer(m_vertex_coords_buffer, vertex_coords_layout);
	}



	IRenderObjectRect2D::~IRenderObjectRect2D()
	{ }
}