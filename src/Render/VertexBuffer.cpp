#include "VertexBuffer.h"

namespace RenderEngine
{
	VertexBuffer::VertexBuffer()
	{ }



	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &m_buffer_id);
	}

	

	VertexBuffer::VertexBuffer(VertexBuffer&& outher_vertex_buffer) noexcept
	{
		m_buffer_id = outher_vertex_buffer.m_buffer_id;
		outher_vertex_buffer.m_buffer_id = 0;
	}



	VertexBuffer& VertexBuffer::operator=(VertexBuffer&& outher_vertex_buffer) noexcept
	{
		if (m_buffer_id != outher_vertex_buffer.m_buffer_id)
		{
			glDeleteBuffers(1, &m_buffer_id);
			m_buffer_id = outher_vertex_buffer.m_buffer_id;
			outher_vertex_buffer.m_buffer_id = 0;
		}
		return *this;
	}



	void VertexBuffer::init(const void* data, const size_t size)
	{
		glGenBuffers(1, &m_buffer_id);
		glBindBuffer(GL_ARRAY_BUFFER, m_buffer_id);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}



	void VertexBuffer::update(const void* data, const size_t size) const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_buffer_id);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}



	void VertexBuffer::bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_buffer_id);
	}



	void VertexBuffer::unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}