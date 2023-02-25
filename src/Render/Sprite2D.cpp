#include "Sprite2D.h"
#include "ShaderProgram.h"
#include "Texture2D.h"
#include "glm/gtc/matrix_transform.hpp"

namespace Renderer
{
	Sprite2D::Sprite2D(std::shared_ptr<Texture2D> ptr_texture,
					   std::shared_ptr<ShaderProgram> ptr_shader_program,
					   const glm::vec2& position,
					   const unsigned int sprite_width,
					   const unsigned int sprite_height,
					   const float angle_rootation) : m_texture(std::move(ptr_texture)), m_shader_program(std::move(ptr_shader_program)),
													  m_position(position), m_sprite_width(sprite_width), m_sprite_height(sprite_height),
													  m_angle(angle_rootation)
	{
		GLfloat points[] = {
			0.0f, 0.0f,
			0.0f, 1.0f,	// first triangle
			1.0f, 1.0f,

			1.0f, 1.0f,
			1.0f, 0.0f,	// second triangle
			0.0f, 0.0f
		};

		GLfloat texture_coords[] = {
			0.0f, 0.0f,
			0.0f, 1.0f,	// first triangle
			1.0f, 1.0f,

			1.0f, 1.0f,
			1.0f, 0.0f,	// second triangle
			0.0f, 0.0f
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

	void Sprite2D::set_position(const glm::vec2& position)
	{
		m_position = position;
	}

	void Sprite2D::set_size(const unsigned int sprite_width, const unsigned int sprite_height)
	{
		m_sprite_width = sprite_width;
		m_sprite_height = sprite_height;
	}

	void Sprite2D::set_rotation(const float angle_rotation)
	{
		m_angle = angle_rotation;
	}

	void Sprite2D::render() const
	{
		glm::mat4 matrix(1.f); // transform matrix
		matrix = glm::translate(matrix, glm::vec3(m_position.x, m_position.y, 0.f));
		matrix = glm::translate(matrix, glm::vec3(0.5f * m_sprite_width, 0.5f * m_sprite_height, 0.f));
		matrix = glm::rotate(matrix, glm::radians(m_angle), glm::vec3(0.f, 0.f, 1.f));
		matrix = glm::translate(matrix, glm::vec3(-0.5f * m_sprite_width, -0.5f * m_sprite_height, 0.f));
		matrix = glm::scale(matrix, glm::vec3(1.f * m_sprite_width, 1.f * m_sprite_height, 1.f));
		
		m_shader_program->use();
		m_shader_program->setInt("tex", 0);
		m_shader_program->setMatrix4("model_matrix", matrix);
		m_shader_program->setMatrix4("clip_matrix", glm::mat4(1.f));

		glBindVertexArray(m_VAO);
		glActiveTexture(GL_TEXTURE0);
		m_texture->bind();
		glDrawArrays(GL_TRIANGLES, 0, 6);
	
		glBindVertexArray(0);
	}
}