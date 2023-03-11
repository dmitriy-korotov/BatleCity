#include "Renderer.h"

namespace RenderEngine
{
	void Renderer::drawElements(const unsigned int primitive_type, const VertexArray& vertex_array,
							   const ElementBuffer& element_buffer, const ShaderProgram& shader_program)
	{
		shader_program.use();
		vertex_array.bind();
		glDrawElements(primitive_type, element_buffer.getCount(), GL_UNSIGNED_INT, nullptr);
	}

	void Renderer::setViewport(const unsigned int width, const unsigned int height,
							   const unsigned int left_offset, const unsigned int bottom_offset)
	{
		glViewport(left_offset, bottom_offset, width, height);
	}

	void Renderer::setDepthTest(const bool enable)
	{
		if (enable)		glEnable(GL_DEPTH_TEST);
		else			glDisable(GL_DEPTH_TEST);
	}

	void Renderer::setClearColor(const float r, const float g, const float b, const float a)
	{
		glClearColor(r, g, b, a);
	}

	void Renderer::clear(const unsigned int buffer_type)
	{
		glClear(buffer_type);
	}

	std::string Renderer::getStringOpenGL(const unsigned int name)
	{
		return (char*)glGetString(name);
	}
}