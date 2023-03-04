#pragma once
#ifndef RENDERER_H
#define RENDERER_H

#include "VertexArray.h"
#include "ElementBuffer.h"
#include "ShaderProgram.h"

namespace RenderEngine
{
	class Renderer
	{
	public:

		static void drawElements(const unsigned int primitive_type, const VertexArray& vertex_array,
								 const ElementBuffer& element_buffer, const ShaderProgram& shader_program);
		static void setClearColor(const float r, const float g, const float b, const float a = 1);
		static void clear(const unsigned int buffer_type = GL_COLOR_BUFFER_BIT);
		static void setViewport(const unsigned int width, const unsigned int height,
								const unsigned int left_offset = 0, const unsigned int bottom_offset = 0);

	};
}

#endif // RENDERER_H