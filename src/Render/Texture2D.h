#pragma once
#ifndef TEXTURE_2D_H
#define TEXTURE_2D_H

#include "glad/glad.h"
#include <string>
#include <glm/mat2x2.hpp>

namespace Renderer
{
	class Texture2D
	{
	public:

		Texture2D(const GLboolean* pixels, const GLuint width, const GLuint height, const GLuint chahhels = 4,
					const GLuint filter = GL_LINEAR, const GLuint wrap_mode = GL_CLAMP_TO_EDGE);
		~Texture2D();

		Texture2D(const Texture2D&) = delete;
		Texture2D& operator=(const Texture2D&) = delete;
		Texture2D(Texture2D&& outher_texture) noexcept;
		Texture2D& operator=(Texture2D&& outher_texture) noexcept;

		void bind() const;

	private:

		GLuint m_mode;
		GLuint m_width;
		GLuint m_height;
		GLuint m_ID;
	};
}

#endif