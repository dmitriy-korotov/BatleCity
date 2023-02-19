#include "ResourceManager.h"

#include <iostream>
#include <fstream>
#include <sstream>

namespace Resources
{
	// PUBLIC
	ResourceManager::ResourceManager(const std::string& executable_path)
	{
		size_t finded_index = executable_path.find_last_of("/\\");

		m_path = executable_path.substr(0, finded_index);
	}

	///////////////////////////////////////////////////////

	std::shared_ptr<Renderer::ShaderProgram> ResourceManager::loadShaderProrgam(const std::string& shader_name, const std::string& relative_path_to_shaders)
	{
		std::string virtex_source = getFileString(m_path + "/" + relative_path_to_shaders + "/vertex.txt");
		if (virtex_source.empty())
		{
			std::cerr << "Can't load vertex shader: " << relative_path_to_shaders + "/vertex.txt" << std::endl;
			return nullptr;
		}

		std::string fragment_source = getFileString(m_path + "/" + relative_path_to_shaders + "/fragment.txt");
		if (fragment_source.empty())
		{
			std::cerr << "Can't load fragment shader: " << relative_path_to_shaders + "/fragment.txt" << std::endl;
			return nullptr;
		}

		return m_shader_programs.emplace(shader_name, std::make_shared<Renderer::ShaderProgram>(virtex_source, fragment_source)).first->second;
	}

	///////////////////////////////////////////////////////

	std::shared_ptr<Renderer::ShaderProgram> ResourceManager::getShaderProgram(const std::string& shader_name) const
	{
		MapShaderProgram::const_iterator shader_program = m_shader_programs.find(shader_name);
		return shader_program->second;
	}

	///////////////////////////////////////////////////////

	// PRIVATE
	std::string ResourceManager::getFileString(const std::string& path)
	{
		std::ifstream file(path, std::ios::in | std::ios::binary);
		if (!file.is_open())
		{
			std::cerr << "Can't load shaders from file: " << path << std::endl;
			return std::string{};
		}
		
		std::stringstream buffer;
		buffer << file.rdbuf();
		return buffer.str();
	}
}