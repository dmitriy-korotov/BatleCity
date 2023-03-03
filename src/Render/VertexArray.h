#pragma once
#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include <glad/glad.h>

namespace Renderer
{
	class VertexBuffer;
	class VertexBufferLayout;

	class VertexArray
	{
	public:

		VertexArray();
		~VertexArray();

		VertexArray(const VertexArray&) = delete;
		VertexArray& operator=(const VertexArray&) = delete;

		VertexArray(VertexArray&& outher_vertex_array) noexcept;
		VertexArray& operator=(VertexArray&& outher_vertex_array) noexcept;

		void addLayoutBuffer(const VertexBuffer& buffer, const VertexBufferLayout& layout_buffer);
		void bind() const;
		void unbind() const;

	private:

		GLuint m_vertex_array_id = 0;
		GLuint m_count_buffers = 0;

	};
}

#endif // VERTEX_ARRAY_H