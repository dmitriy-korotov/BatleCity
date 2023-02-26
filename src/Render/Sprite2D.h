#pragma once
#ifndef SPRITE2D_H
#define SPRITE2D_H

#include <string>
#include <memory>
#include <glm/vec2.hpp>
#include <glad/glad.h>

namespace Renderer
{
	class Texture2D;
	class ShaderProgram;

	class Sprite2D
	{
	public:
		Sprite2D(std::shared_ptr<Texture2D> ptr_texture,
			std::shared_ptr<ShaderProgram> ptr_shader_program,
			std::string subTexture_name = "default",
			const glm::vec2& position = glm::vec2(0, 0),
			const unsigned int sprite_width = 1,
			const unsigned int sprite_height = 1,
			const float angle_rootation = 0);
		~Sprite2D();

		Sprite2D(const Sprite2D&) = delete;
		Sprite2D(Sprite2D&&) = delete;
		Sprite2D& operator=(const Sprite2D&) = delete;
		Sprite2D& operator=(Sprite2D&&) = delete;

		void setPosition(const glm::vec2& position);
		void setSize(const unsigned int width, const unsigned int height);
		void setRotation(const float angle_rotation);
		void render() const;

	private:

		std::shared_ptr<Texture2D> m_texture;
		std::shared_ptr<ShaderProgram> m_shader_program;
		glm::vec2 m_position;
		unsigned int m_sprite_width;
		unsigned int m_sprite_height;
		float m_angle;

		GLuint m_vertex_vbo;
		GLuint m_tex_coords_vbo;
		GLuint m_VAO;
	};
}

#endif