#pragma once
#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include <glad/glad.h>

namespace Renderer
{
	class VertexBuffer
	{
	public:

		VertexBuffer();
		~VertexBuffer();

		VertexBuffer(const VertexBuffer&) = delete;
		VertexBuffer& operator=(const VertexBuffer&) = delete;

		VertexBuffer(VertexBuffer&& outher_vertex_buffer) noexcept;
		VertexBuffer& operator=(VertexBuffer&& outher_vertex_buffer) noexcept;

		void init(const void* data, const size_t size);
		void update(const void* data, const size_t size) const;
		void bind() const;
		void unbind() const;

	private:

		GLuint m_buffer_id = 0;

	};
}

#endif // !VERTEX_BUFFER_H
