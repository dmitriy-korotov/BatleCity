#include "Texture2D.h"
#include "glm/gtc/type_ptr.hpp"

namespace Renderer
{
	Texture2D::Texture2D(const GLboolean* pixels, const GLuint width, const GLuint height,
						const GLuint channels, const GLuint filter, const GLuint wrap_mode) : m_width(width), m_height(height)
	{
		switch (channels)
		{
		case 4:
			m_mode = GL_RGBA;
			break;
		case 3:
			m_mode = GL_RGB;
			break;
		default:
			m_mode = GL_RGBA;
			break;
		}

		glGenTextures(1, &m_ID);
		glBindTexture(GL_TEXTURE_2D, m_ID);
		glTexImage2D(GL_TEXTURE_2D, 0, m_mode, m_width, m_height, 0, m_mode, GL_UNSIGNED_BYTE, pixels);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_mode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_mode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	Texture2D::Texture2D(Texture2D&& outher_texture) noexcept
	{
		m_ID = outher_texture.m_ID;
		outher_texture.m_ID = 0;
		m_width = outher_texture.m_width;
		m_height = outher_texture.m_height;
		m_mode = outher_texture.m_mode;
	}

	Texture2D::~Texture2D()
	{
		glDeleteTextures(1, &m_ID);
	}

	Texture2D& Texture2D::operator=(Texture2D&& outher_texture) noexcept
	{
		glDeleteTextures(1, &m_ID);

		m_ID = outher_texture.m_ID;
		outher_texture.m_ID = 0;
		m_width = outher_texture.m_width;
		m_height = outher_texture.m_height;
		m_mode = outher_texture.m_mode;

		return *this;
	}

	void Texture2D::bind() const
	{
		glBindTexture(GL_TEXTURE_2D, m_ID);
	}
}