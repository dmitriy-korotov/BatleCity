#pragma once
#ifndef ELEMENT_BUFFER_H
#define ELEMENT_BUFFER_H

#include <glad/glad.h>

namespace RenderEngine
{
	class ElementBuffer
	{
	public:

		ElementBuffer();
		~ElementBuffer();

		ElementBuffer(const ElementBuffer&) = delete;
		ElementBuffer& operator=(const ElementBuffer&) = delete;

		ElementBuffer(ElementBuffer&& outher_vertex_buffer) noexcept;
		ElementBuffer& operator=(ElementBuffer&& outher_vertex_buffer) noexcept;

		unsigned int getCount() const { return m_count; }

		void init(const void* data, const unsigned int count);
		void update(const void* data, const size_t size) const;
		void bind() const;
		void unbind() const;

	private:

		GLuint m_buffer_id = 0;
		unsigned int m_count = 0;
	};
}

#endif // !ELEMENT_BUFFER_H
