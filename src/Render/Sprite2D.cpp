#include "Sprite2D.h"

#include "glm/gtc/matrix_transform.hpp"

#include "ShaderProgram.h"
#include "Texture2D.h"
#include "VertexBufferLayout.h"
#include "Renderer.h"

#include <iostream>

namespace RenderEngine
{
	Sprite2D::Sprite2D(std::shared_ptr<Texture2D> ptr_texture,
					   std::shared_ptr<ShaderProgram> ptr_shader_program,
					   std::string subTexture_name) : IRenderObjectRect2D(std::move(ptr_shader_program)), m_texture(std::move(ptr_texture))
	{												  
		RenderEngine::Texture2D::SubTexture2D subTexture = m_texture->getSubTexture(std::move(subTexture_name));

		GLfloat texture_coords[] = {
			//        U                          V
			subTexture.left_bottom_uv.x, subTexture.left_bottom_uv.y,
			subTexture.left_bottom_uv.x, subTexture.right_top_uv.y,			
			subTexture.right_top_uv.x, subTexture.right_top_uv.y,
			subTexture.right_top_uv.x, subTexture.left_bottom_uv.y			
		};

		GLuint indexes[] = {
			0, 1, 2,
			2, 3, 0
		};

		m_VAO.bind();

		m_texture_coords_buffer.init(texture_coords, 2 * 4 * sizeof(GLfloat));
		VertexBufferLayout texture_coords_layout;
		texture_coords_layout.addBufferLayoutElementFloat(2, false);
		m_VAO.addLayoutBuffer(m_texture_coords_buffer, texture_coords_layout);

		m_EBO.init(indexes, 2 * 3 * sizeof(GLuint));
	}



	Sprite2D::~Sprite2D()
	{ }



	bool Sprite2D::isAnimated() const
	{
		return !m_states.empty();
	}




	void Sprite2D::addState(const std::string& state_name, std::vector<std::pair<std::string, double>>& frames)
	{
		m_states.emplace(state_name, std::move(frames));
	}



	double Sprite2D::getDurationStateAnimation(const std::string& state_name) const
	{
		if (!m_states.empty())
		{
			auto state = m_states.find(state_name);
			if (state == m_states.end())
			{
				std::cerr << "Can't find state: " << state_name << std::endl;
				return 0;
			}
			double common_duration = 0;
			for (const auto& frame : (*state).second)
			{
				common_duration += frame.second;
			}
			return common_duration;
		}
		return 0;
	}



	void Sprite2D::render(const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer) const
	{
		glm::mat4 model_matrix(1.f);

		model_matrix = glm::translate(model_matrix, glm::vec3(position, 0.f));
		model_matrix = glm::translate(model_matrix, glm::vec3(0.5f * size, 0.f));
		model_matrix = glm::rotate(model_matrix, glm::radians(rotation), glm::vec3(0.f, 0.f, 1.f));
		model_matrix = glm::translate(model_matrix, glm::vec3(-0.5f * size, 0.f));
		model_matrix = glm::scale(model_matrix, glm::vec3(size, 1.f));

		m_shader_program->use();
		m_shader_program->setInt("tex", 0);
		m_shader_program->setFloat("layer", layer);
		m_shader_program->setMatrix4("model_matrix", model_matrix);

		m_texture->bind();
		Renderer::drawElements(GL_TRIANGLES, m_VAO, m_EBO, *m_shader_program);
	}
}