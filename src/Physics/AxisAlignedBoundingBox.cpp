#include "AxisAlignedBoundingBox.h"

#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

#include "../Render/VertexBufferLayout.h"
#include "../Render/Renderer.h"
#include "../Render/ShaderProgram.h"

#include "../Resources/ResourceManager.h"

namespace Physics
{
	AxisAlignedBoundingBox::AxisAlignedBoundingBox(const glm::vec2& _left_bottom, const glm::vec2& _right_top)
		: left_bottom(_left_bottom)
		, right_top(_right_top)
	{

		GLfloat coords[] = {
			//						coordinates												 colors
						0.f,							0.f,						
						0.f,							1.f,						
						1.f,							1.f,						
						1.f,							0.f						
		};

		GLfloat colors[] = {
			//						coordinates												 colors
						1.f,							0.f,				0.f,
						1.f,							0.f,				0.f,
						1.f,							0.f,				0.f,
						1.f,							0.f,				0.f,
		};

		GLfloat data[] = {
			0.f,	0.f,		1.f,	0.f,	0.f,
			0.f,	1.f,		1.f,	0.f,	0.f,
			1.f,	1.f,		1.f,	0.f,	0.f,
			1.f,	0.f,		1.f,	0.f,	0.f,
		};

		GLuint indexes[] = {
			0, 1, 2, 3, 0
		};

		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);

		/*glGenBuffers(1, &m_coords_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_coords_buffer);
		glBufferData(GL_ARRAY_BUFFER, 2 * 4 * sizeof(GLfloat), &coords, GL_STATIC_DRAW);

		glGenBuffers(1, &m_colors_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_colors_buffer);
		glBufferData(GL_ARRAY_BUFFER, 3 * 4 * sizeof(GLfloat), &colors, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, m_coords_buffer);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

		glBindBuffer(GL_ARRAY_BUFFER, m_colors_buffer);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);*/


		glGenBuffers(1, &m_data_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_data_buffer);
		glBufferData(GL_ARRAY_BUFFER, 5 * 4 * sizeof(GLfloat), &data, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), nullptr);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(2 * sizeof(GLfloat)));


		glGenBuffers(1, &m_element_buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_element_buffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 5 * sizeof(GLuint), indexes, GL_STATIC_DRAW);
	}



	void AxisAlignedBoundingBox::render(const glm::vec2& position, float rotation) const
	{
	    std::shared_ptr<RenderEngine::ShaderProgram> shader_program = Resources::ResourceManager::getShaderProgram("ColliderShaderProgram");

		glm::mat4 model_matrix(1.f); // transform matrix
		model_matrix = glm::translate(model_matrix, glm::vec3(position.x + left_bottom.x, position.y + left_bottom.y, 0.f));
		model_matrix = glm::translate(model_matrix, glm::vec3(0.5f * (right_top.x - left_bottom.x), 0.5f * (right_top.y - left_bottom.y), 0.f));
		model_matrix = glm::rotate(model_matrix, glm::radians(rotation), glm::vec3(0.f, 0.f, 1.f));
		model_matrix = glm::translate(model_matrix, glm::vec3(-0.5f * (right_top.x - left_bottom.x), -0.5f * (right_top.y - left_bottom.y), 0.f));
		model_matrix = glm::scale(model_matrix, glm::vec3(1.f * (right_top.x - left_bottom.x), 1.f * (right_top.y - left_bottom.y), 1.f));

		shader_program->use();
		shader_program->setMatrix4("model_matrix", model_matrix);

		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_element_buffer);
		glDrawElements(GL_LINE_STRIP, 5, GL_UNSIGNED_INT, 0);
	}



	AxisAlignedBoundingBox::~AxisAlignedBoundingBox()
	{
		glDeleteBuffers(1, &m_element_buffer);
		glDeleteBuffers(1, &m_coords_buffer);
		glDeleteBuffers(1, &m_colors_buffer);
		glDeleteBuffers(1, &m_data_buffer);
		glDeleteVertexArrays(1, &m_VAO);
	}
}