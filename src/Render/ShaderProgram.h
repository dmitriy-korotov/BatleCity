#pragma once
#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <glad/glad.h>
#include <string>
#include "glm/mat4x4.hpp"

namespace Renderer
{
	class ShaderProgram
	{
	public:

		ShaderProgram(const std::string& vertex_shader, const std::string& fragment_shader);

		~ShaderProgram();

		inline bool isCompiled() const { return m_is_compiled; }

		void use() const;

		ShaderProgram() = delete;
		ShaderProgram(const ShaderProgram&) = delete;
		ShaderProgram(ShaderProgram&& outher_shader_program) noexcept;
		ShaderProgram& operator=(const ShaderProgram&) = delete;
		ShaderProgram& operator=(ShaderProgram&& outher_shader_program) noexcept;

		void setInt(const std::string& name, GLuint value) const;
		void setMatrix4(const std::string& name, const glm::mat4& matrix) const;

	private:

		// VARIEBLES
		bool m_is_compiled;
		GLuint m_ID;

		// PRIVATE METHODS
		bool CreateShader(const std::string& source_code, const GLuint type_shder, GLuint& shder_ID);
	};
}

#endif