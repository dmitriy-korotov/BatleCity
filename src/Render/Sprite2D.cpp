#include "Sprite2D.h"
#include "ShaderProgram.h"
#include "Texture2D.h"
#include "glm/gtc/matrix_transform.hpp"

namespace Renderer
{
	Sprite2D::Sprite2D(std::shared_ptr<Texture2D> ptr_texture,
					   std::shared_ptr<ShaderProgram> ptr_shader_program,
					   std::string subTexture_name,
					   const glm::vec2& position,
					   const unsigned int sprite_width,
					   const unsigned int sprite_height,
					   const float angle_rootation) : m_texture(std::move(ptr_texture)), m_shader_program(std::move(ptr_shader_program)),
													  m_position(position), m_sprite_width(sprite_width), m_sprite_height(sprite_height),
													  m_angle(angle_rootation)
	{
		Renderer::Texture2D::SubTexture2D subTexture = m_texture->getSubTexture(std::move(subTexture_name));

		GLfloat points[] = {
			0.f, 0.f,
			0.f, 1.f,		// first triangle
			1.f, 1.f,

			1.f, 1.f,
			1.f, 0.f,		// second triangle
			0.f, 0.f
		};

		GLfloat texture_coords[] = {
			subTexture.left_bottom_uv.x, subTexture.left_bottom_uv.y,
			subTexture.left_bottom_uv.x, subTexture.right_top_uv.y,			// first triangle
			subTexture.right_top_uv.x, subTexture.right_top_uv.y,

			subTexture.right_top_uv.x, subTexture.right_top_uv.y,
			subTexture.right_top_uv.x, subTexture.left_bottom_uv.y,			// second triangle
			subTexture.left_bottom_uv.x, subTexture.left_bottom_uv.y,
		};

		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);

		glGenBuffers(1, &m_vertex_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertex_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

		glGenBuffers(1, &m_tex_coords_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_tex_coords_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(texture_coords), &texture_coords, GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	Sprite2D::~Sprite2D()
	{
		glDeleteBuffers(1, &m_vertex_vbo);
		glDeleteBuffers(1, &m_tex_coords_vbo);
		glDeleteVertexArrays(1, &m_VAO);
	}

	void Sprite2D::setPosition(const glm::vec2& position)
	{
		m_position = position;
	}

	void Sprite2D::setSize(const unsigned int sprite_width, const unsigned int sprite_height)
	{
		m_sprite_width = sprite_width;
		m_sprite_height = sprite_height;
	}

	void Sprite2D::setRotation(const float angle_rotation)
	{
		m_angle = angle_rotation;
	}

	void Sprite2D::render() const
	{
		glm::mat4 model_matrix(1.f); // transform matrix
		model_matrix = glm::translate(model_matrix, glm::vec3(m_position.x, m_position.y, 0.f));
		model_matrix = glm::translate(model_matrix, glm::vec3(0.5f * m_sprite_width, 0.5f * m_sprite_height, 0.f));
		model_matrix = glm::rotate(model_matrix, glm::radians(m_angle), glm::vec3(0.f, 0.f, 1.f));
		model_matrix = glm::translate(model_matrix, glm::vec3(-0.5f * m_sprite_width, -0.5f * m_sprite_height, 0.f));
		model_matrix = glm::scale(model_matrix, glm::vec3(1.f * m_sprite_width, 1.f * m_sprite_height, 1.f));

		glm::mat4 projection_matrix = glm::ortho(0.f, static_cast<float>(1024), 0.f, static_cast<float>(720), -100.f, 100.f);
		
		m_shader_program->use();
		m_shader_program->setInt("tex", 0);
		m_shader_program->setMatrix4("model_matrix", model_matrix);
		m_shader_program->setMatrix4("clip_matrix", projection_matrix);

		glBindVertexArray(m_VAO);
		glActiveTexture(GL_TEXTURE0);
		m_texture->bind();
		glDrawArrays(GL_TRIANGLES, 0, 6);
	
		glBindVertexArray(0);
	}
}