#pragma once
#ifndef SPRITE2D_H
#define SPRITE2D_H

#include <string>
#include <memory>
#include <glm/vec2.hpp>
#include <glad/glad.h>

#include "VertexBuffer.h"
#include "ElementBuffer.h"
#include "VertexArray.h"

namespace RenderEngine
{
	class Texture2D;
	class ShaderProgram;

	class Sprite2D
	{
	public:
		Sprite2D(std::shared_ptr<Texture2D> ptr_texture,
				 std::shared_ptr<ShaderProgram> ptr_shader_program,
				 std::string subTexture_name = "default");
		~Sprite2D();

		Sprite2D(const Sprite2D&) = delete;
		Sprite2D(Sprite2D&&) = delete;
		Sprite2D& operator=(const Sprite2D&) = delete;
		Sprite2D& operator=(Sprite2D&&) = delete;

		virtual void render(const glm::vec2& position, const glm::vec2& size, const float rotation) const;

	protected:

		std::shared_ptr<Texture2D> m_texture;
		std::shared_ptr<ShaderProgram> m_shader_program;
		
		VertexArray m_VAO;
		VertexBuffer m_vertex_coords_buffer;
		VertexBuffer m_texture_coords_buffer;
		ElementBuffer m_EBO;
	};
}

#endif