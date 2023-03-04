#include "VertexBufferLayout.h"

namespace RenderEngine
{
	void VertexBufferLayout::reserve(const size_t count)
	{
		m_layout_elements_of_buffer.reserve(count);
	}



	void VertexBufferLayout::addBufferLayoutElementFloat(const GLuint count, const GLboolean normalized)
	{
		m_layout_elements_of_buffer.push_back({ count, GL_FLOAT, normalized, static_cast<unsigned int>(count * sizeof(GLfloat))});
		m_stride += m_layout_elements_of_buffer.back().count * sizeof(GLfloat);
	}



	const std::vector<VertexBufferLayoutElement>& VertexBufferLayout::getLayoutElements() const
	{
		return m_layout_elements_of_buffer;
	}
}