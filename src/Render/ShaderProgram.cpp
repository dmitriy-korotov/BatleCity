#include "ShaderProgram.h"

#include <iostream>

namespace RenderEngine
{
	// PUBLIC
	ShaderProgram::ShaderProgram(const std::string& vertex_shader, const std::string& fragment_shader) : m_is_compiled(false), m_ID(0)
	{
		// CREATE SHDERS
		GLuint vertex_shader_ID;
		if (!CreateShader(vertex_shader, GL_VERTEX_SHADER, vertex_shader_ID))
		{
			std::cerr << "VERTEX SHDER Compile-time error" << std::endl;
			return;
		}

		GLuint fragment_shader_ID;
		if (!CreateShader(fragment_shader, GL_FRAGMENT_SHADER, fragment_shader_ID))
		{
			glDeleteShader(vertex_shader_ID);
			std::cerr << "FRAGMENT SHADER Compile-time error" << std::endl;
			return;
		}

		// CREATE SHADER PROGRAM
		m_ID = glCreateProgram();
		glAttachShader(m_ID, vertex_shader_ID);
		glAttachShader(m_ID, fragment_shader_ID);
		glLinkProgram(m_ID);

		GLint success;
		glGetProgramiv(m_ID, GL_LINK_STATUS, &success);

		if (!success)
		{
			GLchar info_log[1024];
			glGetProgramInfoLog(m_ID, 1024, nullptr, info_log);
			std::cerr << "ERROR::SHADER: Link-time error:\n" << info_log << std::endl;
		}
		else
		{
			m_is_compiled = true;
		}

		glDeleteShader(vertex_shader_ID);
		glDeleteShader(fragment_shader_ID);
	}

	ShaderProgram::ShaderProgram(ShaderProgram&& outher_shader_program) noexcept
	{
		m_is_compiled = outher_shader_program.m_is_compiled;
		m_ID = outher_shader_program.m_ID;

		outher_shader_program.m_is_compiled = false;
		outher_shader_program.m_ID = 0;
	}

	ShaderProgram& ShaderProgram::operator=(ShaderProgram&& outher_shader_program) noexcept
	{
		glDeleteProgram(m_ID);

		m_is_compiled = outher_shader_program.m_is_compiled;
		m_ID = outher_shader_program.m_ID;

		outher_shader_program.m_is_compiled = false;
		outher_shader_program.m_ID = 0;

		return *this;
	}

	void ShaderProgram::use() const
	{
		glUseProgram(m_ID);
	}

	ShaderProgram::~ShaderProgram()
	{
		glDeleteProgram(m_ID);
	}

	void ShaderProgram::setInt(const std::string& name, const GLuint value) const
	{
		glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
	}

	void ShaderProgram::setFloat(const std::string& name, const GLfloat value) const
	{
		glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
	}

	void ShaderProgram::setMatrix4(const std::string& name, const glm::mat4& matrix) const
	{
		glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &matrix[0][0]);
	}

	// PRIVATE
	bool ShaderProgram::CreateShader(const std::string& source_code, const GLuint type_shader, GLuint& shader_ID)
	{
		shader_ID = glCreateShader(type_shader);
		const char* code = source_code.c_str();
		glShaderSource(shader_ID, 1, &code, nullptr);
		glCompileShader(shader_ID);

		GLint success;
		glGetShaderiv(shader_ID, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			GLchar info_log[1024];
			glGetShaderInfoLog(shader_ID, 1024, nullptr, info_log);
			std::cerr << "ERROR::SHADER: Compile-time error:\n" << info_log << std::endl;
			return false;
		}
		return true;
	}
}