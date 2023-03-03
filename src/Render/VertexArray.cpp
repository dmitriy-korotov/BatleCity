#include "VertexArray.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

namespace Renderer
{
	VertexArray::VertexArray()
	{
		glGenVertexArrays(1, &m_vertex_array_id);
	}



	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &m_vertex_array_id);
	}



	VertexArray::VertexArray(VertexArray&& outher_vertex_array) noexcept
	{
		m_vertex_array_id = outher_vertex_array.m_vertex_array_id;
		outher_vertex_array.m_vertex_array_id = 0;
	}



	VertexArray& VertexArray::operator=(VertexArray&& outher_vertex_array) noexcept
	{
		if (m_vertex_array_id != outher_vertex_array.m_vertex_array_id)
		{
			glDeleteVertexArrays(1, &m_vertex_array_id);
			m_vertex_array_id = outher_vertex_array.m_vertex_array_id;
			outher_vertex_array.m_vertex_array_id;
		}
		return *this;
	}



	void VertexArray::addLayoutBuffer(const VertexBuffer& buffer, const VertexBufferLayout& layout_buffer)
	{
		bind();
		buffer.bind();

		std::vector<VertexBufferLayoutElement> layout_elements = layout_buffer.getLayoutElements();

		for (size_t i = 0; i < layout_elements.size(); ++i)
		{
			VertexBufferLayoutElement current_layout_element = layout_elements[i];
			GLuint current_attrib_index = m_count_buffers + i;
			unsigned int* offset = nullptr;
			glEnableVertexAttribArray(current_attrib_index);
			glVertexAttribPointer(current_attrib_index, current_layout_element.count, current_layout_element.type,
								  current_layout_element.normalized, layout_buffer.getStride(), offset);
			offset += current_layout_element.size;
		}
		m_count_buffers += layout_elements.size();
	}



	void VertexArray::bind() const
	{
		glBindVertexArray(m_vertex_array_id);
	}



	void VertexArray::unbind() const
	{
		glBindVertexArray(0);
	}
}