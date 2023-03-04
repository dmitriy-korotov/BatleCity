#pragma once
#ifndef TEXTURE_2D_H
#define TEXTURE_2D_H

#include "glad/glad.h"
#include <string>
#include <glm/mat2x2.hpp>
#include <map>

namespace RenderEngine
{
	class Texture2D
	{
	public:


		struct SubTexture2D
		{
		public:

			SubTexture2D();
			SubTexture2D(const glm::vec2& _left_bottom_uv, const glm::vec2& _right_top_uv);

			glm::vec2 left_bottom_uv;
			glm::vec2 right_top_uv;
		
		};


		Texture2D(const GLboolean* pixels, const GLuint width, const GLuint height, const GLuint chahhels = 4,
					const GLuint filter = GL_LINEAR, const GLuint wrap_mode = GL_CLAMP_TO_EDGE);
		~Texture2D();

		Texture2D(const Texture2D&) = delete;
		Texture2D& operator=(const Texture2D&) = delete;
		Texture2D(Texture2D&& outher_texture) noexcept;
		Texture2D& operator=(Texture2D&& outher_texture) noexcept;

		inline unsigned int width() const { return m_width; }
		inline unsigned int height() const { return m_height; }
		inline unsigned int channels() const { return m_mode; }

		void bind() const;
		void addSubTexture(const std::string& subTexture_name, const glm::vec2& left_bottom_uv, const glm::vec2& right_top_uv);
		const SubTexture2D& getSubTexture(const std::string& subTexture_name) const;

	private:

		GLuint m_mode;
		GLuint m_width;
		GLuint m_height;
		GLuint m_ID;

		typedef std::map<std::string, SubTexture2D> MapSubTextures;
		MapSubTextures subTextures;
	};
}

#endif