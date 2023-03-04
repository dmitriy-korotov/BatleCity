#include "Sprite2D.h"

#include "glm/gtc/matrix_transform.hpp"

#include "ShaderProgram.h"
#include "Texture2D.h"
#include "VertexBufferLayout.h"
#include "Renderer.h"

namespace RenderEngine
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
		RenderEngine::Texture2D::SubTexture2D subTexture = m_texture->getSubTexture(std::move(subTexture_name));

		GLfloat vertex_coords[] = {
			0.f, 0.f,
			0.f, 1.f,		
			1.f, 1.f,
			1.f, 0.f,	
		};

		GLfloat texture_coords[] = {
			//        U                          V
			subTexture.left_bottom_uv.x, subTexture.left_bottom_uv.y,
			subTexture.left_bottom_uv.x, subTexture.right_top_uv.y,			
			subTexture.right_top_uv.x, subTexture.right_top_uv.y,
			subTexture.right_top_uv.x, subTexture.left_bottom_uv.y,			
		};

		GLuint indexes[] = {
			0, 1, 2,
			2, 3, 0
		};

		m_VAO.bind();
		//					2 - amount coords in one point;		4 - amount points
		m_vertex_coords_buffer.init(vertex_coords, 2 * 4 * sizeof(GLfloat));
		VertexBufferLayout vertex_coords_layout;
		vertex_coords_layout.addBufferLayoutElementFloat(2, false);
		m_VAO.addLayoutBuffer(m_vertex_coords_buffer, vertex_coords_layout);

		m_texture_coords_buffer.init(texture_coords, 2 * 4 * sizeof(GLfloat));
		VertexBufferLayout texture_coords_layout;
		texture_coords_layout.addBufferLayoutElementFloat(2, false);
		m_VAO.addLayoutBuffer(m_texture_coords_buffer, texture_coords_layout);

		m_EBO.init(indexes, 2 * 3 * sizeof(GLuint));

		m_texture_coords_buffer.unbind();
		m_VAO.unbind();
		m_EBO.unbind();
	}



	Sprite2D::~Sprite2D()
	{ }



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

		m_VAO.bind();
		glActiveTexture(GL_TEXTURE0);
		m_texture->bind();
		Renderer::drawElements(GL_TRIANGLES, m_VAO, m_EBO, *m_shader_program);

		m_VAO.unbind();
	}
}