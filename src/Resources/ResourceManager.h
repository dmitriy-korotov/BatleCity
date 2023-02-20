#pragma once
#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <string>
#include <map>
#include <memory>
#include "../Render/ShaderProgram.h"

namespace Resources
{
	class ResourceManager
	{
	public:

		ResourceManager(const std::string& executable_path);

		ResourceManager(const ResourceManager&) = delete;
		ResourceManager(ResourceManager&&) = delete;
		ResourceManager& operator=(const ResourceManager&) = delete;
		ResourceManager& operator=(ResourceManager&&) = delete;
		
		~ResourceManager() = default;

		std::shared_ptr<Renderer::ShaderProgram> loadShaderProrgam(const std::string& shader_name, const std::string& relative_path_to_shaders);
	
		std::shared_ptr<Renderer::ShaderProgram> getShaderProgram(const std::string& shader_name) const;

		void loadTexture(const std::string& texture_name, const std::string& relative_path_to_texture);

	private:

		static std::string getFileString(const std::string& path);

		// VARIEBLES
		typedef std::map<std::string, std::shared_ptr<Renderer::ShaderProgram>> MapShaderProgram;

		MapShaderProgram m_shader_programs;

		std::string m_path;
	};
}

#endif // !RESOURCE_MANAGER_H
