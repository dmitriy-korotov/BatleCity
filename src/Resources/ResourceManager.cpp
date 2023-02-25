#include "ResourceManager.h"

#include "../Render/ShaderProgram.h"
#include "../Render/Texture2D.h"
#include "../Render/Sprite2D.h"
#include <iostream>
#include <fstream>
#include <sstream>
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "stb_image.h"

namespace Resources
{
	// PUBLIC
	ResourceManager::ResourceManager(const std::string& executable_path)
	{
		size_t finded_index = executable_path.find_last_of("/\\");

		m_path = executable_path.substr(0, finded_index);
	}

	///////////////////////////////////////////////////////

	std::shared_ptr<Renderer::ShaderProgram> ResourceManager::loadShaderProrgam(const std::string& shader_name,
																				const std::string& path_to_vertex_shader_source,
																				const std::string& path_to_fragment_shader_source)
	{
		std::string virtex_source = getFileString(m_path + "/" + path_to_vertex_shader_source);
		if (virtex_source.empty())
		{
			std::cerr << "Can't load vertex shader: " << path_to_vertex_shader_source << std::endl;
			return nullptr;
		}

		std::string fragment_source = getFileString(m_path + "/" + path_to_fragment_shader_source);
		if (fragment_source.empty())
		{
			std::cerr << "Can't load fragment shader: " << path_to_fragment_shader_source << std::endl;
			return nullptr;
		}

		return m_shader_programs.emplace(shader_name, std::make_shared<Renderer::ShaderProgram>(virtex_source, fragment_source)).first->second;
	}

	///////////////////////////////////////////////////////

	std::shared_ptr<Renderer::Texture2D> ResourceManager::loadTexture(const std::string& texture_name, const std::string& relative_path_to_texture)
	{
		int channels = 0;
		int width = 0;
		int height = 0;

		stbi_set_flip_vertically_on_load(true);
		unsigned char* pixels = stbi_load((m_path + "/" + relative_path_to_texture).c_str(), &width, &height, &channels, 0);

		if (!pixels)
		{
			std::cerr << "Can't load texture: " << relative_path_to_texture << std::endl;
		}

		std::shared_ptr<Renderer::Texture2D> new_texture = m_textures.emplace(texture_name,
			std::make_shared<Renderer::Texture2D>(pixels, width, height, channels, GL_NEAREST, GL_CLAMP_TO_EDGE)).first->second;

		stbi_image_free(pixels);

		return new_texture;
	}

	///////////////////////////////////////////////////////

	std::shared_ptr<Renderer::ShaderProgram> ResourceManager::getShaderProgram(const std::string& shader_name) const
	{
		MapShaderProgram::const_iterator it = m_shader_programs.find(shader_name);

		if (it == m_shader_programs.end())
		{
			std::cerr << "Can't find shader program: " << shader_name << std::endl;
			return nullptr;
		}

		return it->second;
	}

	std::shared_ptr<Renderer::Texture2D> ResourceManager::getTexture(const std::string& texture_name) const
	{
		MapTexture2D::const_iterator it = m_textures.find(texture_name);

		if (it == m_textures.end())
		{
			std::cerr << "Can't find texture: " << texture_name << std::endl;
			return nullptr;
		}
		return it->second;
	}

	///////////////////////////////////////////////////////

	std::shared_ptr<Renderer::Sprite2D> ResourceManager::loadSprite(const std::string& sprite_name,
																	const std::string& shader_program_name,
																	const std::string& texture_name,
																	const unsigned int sprite_width,
																	const unsigned int sprite_height)
	{
		std::shared_ptr<Renderer::ShaderProgram> shader_program = getShaderProgram(shader_program_name);
		if (shader_program == nullptr)
		{
			std::cerr << "Can't load sprite: " << sprite_name << std::endl;
			return nullptr;
		}

		std::shared_ptr<Renderer::Texture2D> texture = getTexture(texture_name);
		if (texture == nullptr)
		{
			std::cerr << "Can't load sptite: " << sprite_name << std::endl;
			return nullptr;
		}
		
		return m_sprites.emplace(sprite_name, std::make_shared<Renderer::Sprite2D>(texture,
																				   shader_program,
																				   glm::vec2(0.f, 0.f),
																				   sprite_width,
																				   sprite_height)).first->second;
	}

	//////////////////////////////////////////////////////

	std::shared_ptr<Renderer::Sprite2D> ResourceManager::getsprite(const std::string& sprite_name)
	{
		MapSprite2D::const_iterator it = m_sprites.find(sprite_name);
		
		if (it == m_sprites.end())
		{
			std::cerr << "Can't find sprite: " << sprite_name << std::endl;
			return nullptr;
		}
		return it->second;
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