#include "Texture2D.h"
#include "glm/gtc/type_ptr.hpp"

namespace Renderer
{

	///////////////////////////////////////////////////////////
	//					  SUB TEXTURE
	///////////////////////////////////////////////////////////

	Texture2D::SubTexture2D::SubTexture2D() : left_bottom_uv(0.f, 0.f), right_top_uv(1.f, 1.f)
	{ }

	Texture2D::SubTexture2D::SubTexture2D(const glm::vec2& _left_bottom_uv, const glm::vec2& _right_top_uv)
		: left_bottom_uv(_left_bottom_uv), right_top_uv(_right_top_uv)
	{ }

	///////////////////////////////////////////////////////////
	//					   TEXTURE
	///////////////////////////////////////////////////////////

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

	void Texture2D::addSubTexture(const std::string& subTexture_name, const glm::vec2& left_bottom_uv, const glm::vec2& right_top_uv)
	{
		SubTexture2D new_subTexture(left_bottom_uv, right_top_uv);
		subTextures.emplace(subTexture_name, std::move(new_subTexture));
	}

	const Texture2D::SubTexture2D& Texture2D::getSubTexture(const std::string& subTexture_name) const
	{
		MapSubTextures::const_iterator it = subTextures.find(subTexture_name);
		if (it != subTextures.end())
		{
			return it->second;
		}
		const static SubTexture2D default_subTexture;
		return default_subTexture;
	}
}