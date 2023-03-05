#pragma once
#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <string>
#include <map>
#include <vector>
#include <memory>

#include "../rapidjson/document.h"

namespace RenderEngine
{
	class ShaderProgram;
	class Texture2D;
	class Sprite2D;
	class subTexture2D;
	class AnimatedSprite2D;
}

namespace Resources
{
	class ResourceManager
	{
	public:

		static void setExecutablePath(const std::string& executable_path);
		static void unloadAllResources();

		ResourceManager() = delete;
		ResourceManager(const ResourceManager&) = delete;
		ResourceManager(ResourceManager&&) = delete;
		ResourceManager& operator=(const ResourceManager&) = delete;
		ResourceManager& operator=(ResourceManager&&) = delete;
		~ResourceManager() = delete;

		static std::shared_ptr<RenderEngine::ShaderProgram> loadShaderProrgam(const std::string& shader_name,
																		  const std::string& path_to_vertex_shader_source, 
																		  const std::string& path_to_fragment_shader_source);
		static std::shared_ptr<RenderEngine::ShaderProgram> getShaderProgram(const std::string& shader_name);



		static std::shared_ptr<RenderEngine::Texture2D> loadTexture(const std::string& texture_name, const std::string& relative_path_to_texture);
		static std::shared_ptr<RenderEngine::Texture2D> getTexture(const std::string& texture_name);



		static std::shared_ptr<RenderEngine::Sprite2D> loadSprite(const std::string& sprite_name,
															  const std::string& shader_program_name,
															  const std::string& texture_name,
															  const unsigned int sprite_width = 1,
															  const unsigned int sprite_height = 1,
															  const std::string& subTexture_name = "default");
		static std::shared_ptr<RenderEngine::Sprite2D> getSprite(const std::string& sprite_name);



		static std::shared_ptr<RenderEngine::AnimatedSprite2D> loadAnimatedSprite(const std::string& sprite_name,
																			  const std::string& shader_program_name,
																			  const std::string& texture_name,
																			  const unsigned int sprite_width = 1,
																			  const unsigned int sprite_height = 1,
																			  const std::string& subTexture_name = "default");
		static std::shared_ptr<RenderEngine::AnimatedSprite2D> getAnimatedSprite(const std::string& sprite_name);



		static std::shared_ptr<RenderEngine::Texture2D> loadTextureAtlas(const std::string& texture_name,
																	 const std::vector<std::string> subTexture_names,
																	 const std::string& relative_path_to_texture,
																	 const unsigned int width_subTexture,
																	 const unsigned int height_subtexture);

		static bool loadResourcesJSON(const std::string& path_to_JSON_file);

	private:

		static std::string getFileString(const std::string& path);
		static bool loadShaderProgramsJSON(const rapidjson::Document& document);
		static bool loadTextureAtlasesJSON(const rapidjson::Document& document);
		static bool loadAnimatedSpritesJSON(const rapidjson::Document& document);

		// VARIEBLES
		typedef std::map<std::string, std::shared_ptr<RenderEngine::ShaderProgram>> MapShaderProgram;
		static MapShaderProgram m_shader_programs;

		typedef std::map<std::string, std::shared_ptr<RenderEngine::Texture2D>> MapTexture2D;
		static MapTexture2D m_textures;

		typedef std::map<std::string, std::shared_ptr<RenderEngine::Sprite2D>> MapSprite2D;
		static MapSprite2D m_sprites;

		typedef std::map<std::string, std::shared_ptr<RenderEngine::AnimatedSprite2D>> MapAnimatedSprite2D;
		static MapAnimatedSprite2D m_animated_sprites;

		static std::string m_path;
	};
}

#endif // !RESOURCE_MANAGER_H
