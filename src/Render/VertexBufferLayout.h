#pragma once
#ifndef VERTEX_BUFFER_LAYOUT_H
#define VERTEX_BUFFER_LAYOUT_H

#include <glad/glad.h>
#include <vector>

namespace Renderer
{
	struct VertexBufferLayoutElement
	{
		GLuint count;
		GLuint type;
		GLboolean normalized;
		unsigned int size;
	};

	class VertexBufferLayout
	{
	public:


		VertexBufferLayout() = default;
		~VertexBufferLayout() = default;

		inline GLuint getStride() const { return m_stride; }
		void reserve(const size_t count);
		void addBufferLayoutElementFloat(const GLuint count, const GLboolean normalized);
		const std::vector<VertexBufferLayoutElement>& getLayoutElements() const;

	private:

		std::vector<VertexBufferLayoutElement> m_layout_elements_of_buffer;
		GLuint m_stride = 0;

	};
}

#endif // VERTEX_BUFFER_LAYOUT_H