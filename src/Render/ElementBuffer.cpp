#include "ElementBuffer.h"

namespace RenderEngine
{
	ElementBuffer::ElementBuffer()
	{ }



	ElementBuffer::~ElementBuffer()
	{
		glDeleteBuffers(1, &m_buffer_id);
	}



	ElementBuffer::ElementBuffer(ElementBuffer&& outher_vertex_buffer) noexcept
	{
		m_buffer_id = outher_vertex_buffer.m_buffer_id;
		outher_vertex_buffer.m_buffer_id = 0;
	}



	ElementBuffer& ElementBuffer::operator=(ElementBuffer&& outher_vertex_buffer) noexcept
	{
		if (m_buffer_id != outher_vertex_buffer.m_buffer_id)
		{
			glDeleteBuffers(1, &m_buffer_id);
			m_buffer_id = outher_vertex_buffer.m_buffer_id;
			outher_vertex_buffer.m_buffer_id = 0;
		}
		return *this;
	}



	void ElementBuffer::init(const void* data, const unsigned int count)
	{
		m_count = count;

		glGenBuffers(1, &m_buffer_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
	}



	void ElementBuffer::update(const void* data, const size_t size) const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer_id);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size, data);
	}



	void ElementBuffer::bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer_id);
	}



	void ElementBuffer::unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}