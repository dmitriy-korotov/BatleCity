#include "ResourceManager.h"

#include "../Render/ShaderProgram.h"
#include "../Render/Texture2D.h"
#include "../Render/Sprite2D.h"
#include "../Render/AnimatedSprite2D.h"

#include <iostream>
#include <fstream>
#include <sstream>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "stb_image.h"
#include "../rapidjson/error/en.h"

namespace Resources
{
	// STATIC VARIEBLES
	ResourceManager::MapShaderProgram ResourceManager::m_shader_programs;
	ResourceManager::MapTexture2D ResourceManager::m_textures;
	ResourceManager::MapSprite2D ResourceManager::m_sprites;
	ResourceManager::MapAnimatedSprite2D ResourceManager::m_animated_sprites;
	std::string ResourceManager::m_path;


	// PUBLIC
	void ResourceManager::setExecutablePath(const std::string& executable_path)
	{
		size_t finded_index = executable_path.find_last_of("/\\");

		m_path = executable_path.substr(0, finded_index);
	}



	void ResourceManager::unloadAllResources()
	{
		m_animated_sprites.clear();
		m_shader_programs.clear();
		m_sprites.clear();
		m_textures.clear();
	}

	

	std::shared_ptr<RenderEngine::ShaderProgram> ResourceManager::loadShaderProrgam(const std::string& shader_name,
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

		return m_shader_programs.emplace(shader_name, std::make_shared<RenderEngine::ShaderProgram>(virtex_source, fragment_source)).first->second;
	}



	std::shared_ptr<RenderEngine::Texture2D> ResourceManager::loadTexture(const std::string& texture_name, const std::string& relative_path_to_texture)
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

		std::shared_ptr<RenderEngine::Texture2D> new_texture = m_textures.emplace(texture_name,
			std::make_shared<RenderEngine::Texture2D>(pixels, width, height, channels, GL_NEAREST, GL_CLAMP_TO_EDGE)).first->second;

		stbi_image_free(pixels);

		return new_texture;
	}



	std::shared_ptr<RenderEngine::ShaderProgram> ResourceManager::getShaderProgram(const std::string& shader_name)
	{
		MapShaderProgram::const_iterator it = m_shader_programs.find(shader_name);

		if (it == m_shader_programs.end())
		{
			std::cerr << "Can't find shader program: " << shader_name << std::endl;
			return nullptr;
		}

		return it->second;
	}



	std::shared_ptr<RenderEngine::Texture2D> ResourceManager::getTexture(const std::string& texture_name)
	{
		MapTexture2D::const_iterator it = m_textures.find(texture_name);

		if (it == m_textures.end())
		{
			std::cerr << "Can't find texture: " << texture_name << std::endl;
			return nullptr;
		}
		return it->second;
	}

	

	std::shared_ptr<RenderEngine::Sprite2D> ResourceManager::loadSprite(const std::string& sprite_name,
																		const std::string& shader_program_name,
																		const std::string& texture_name,
																		const unsigned int sprite_width,
																		const unsigned int sprite_height,
																		const std::string& subTexture_name)
	{
		std::shared_ptr<RenderEngine::ShaderProgram> shader_program = getShaderProgram(shader_program_name);
		if (shader_program == nullptr)
		{
			std::cerr << "Can't load sprite: " << sprite_name << std::endl;
			return nullptr;
		}

		std::shared_ptr<RenderEngine::Texture2D> texture = getTexture(texture_name);
		if (texture == nullptr)
		{
			std::cerr << "Can't load sptite: " << sprite_name << std::endl;
			return nullptr;
		}
		
		return m_sprites.emplace(sprite_name, std::make_shared<RenderEngine::Sprite2D>(texture,
																				   shader_program,
																				   subTexture_name,
																				   glm::vec2(0.f, 0.f),
																				   sprite_width,
																				   sprite_height)).first->second;
	}



	std::shared_ptr<RenderEngine::Sprite2D> ResourceManager::getSprite(const std::string& sprite_name)
	{
		MapSprite2D::const_iterator it = m_sprites.find(sprite_name);
		
		if (it == m_sprites.end())
		{
			std::cerr << "Can't find sprite: " << sprite_name << std::endl;
			return nullptr;
		}
		return it->second;
	}



	std::shared_ptr<RenderEngine::AnimatedSprite2D> ResourceManager::loadAnimatedSprite(const std::string& sprite_name,
																					    const std::string& shader_program_name,
																						const std::string& texture_name,
																						const unsigned int sprite_width,
																						const unsigned int sprite_height,
																						const std::string& subTexture_name)
	{
		std::shared_ptr<RenderEngine::ShaderProgram> shader_program = getShaderProgram(shader_program_name);
		if (shader_program == nullptr)
		{
			std::cerr << "Can't load sprite: " << sprite_name << std::endl;
			return nullptr;
		}

		std::shared_ptr<RenderEngine::Texture2D> texture = getTexture(texture_name);
		if (texture == nullptr)
		{
			std::cerr << "Can't load sptite: " << sprite_name << std::endl;
			return nullptr;
		}

		return m_animated_sprites.emplace(sprite_name, std::make_shared<RenderEngine::AnimatedSprite2D>(texture,
																						    shader_program,
																							subTexture_name,
																							glm::vec2(0.f, 0.f),
																							sprite_width,
																							sprite_height)).first->second;
	}

	

	std::shared_ptr<RenderEngine::AnimatedSprite2D> ResourceManager::getAnimatedSprite(const std::string& sprite_name)
	{
		MapAnimatedSprite2D::const_iterator it = m_animated_sprites.find(sprite_name);

		if (it == m_animated_sprites.end())
		{
			std::cerr << "Can't find animation sprite: " << sprite_name << std::endl;
			return nullptr;
		}
		return it->second;
	}



	std::shared_ptr<RenderEngine::Texture2D> ResourceManager::loadTextureAtlas(const std::string& texture_name,
																			   const std::vector<std::string> subTexture_names,
																			   const std::string& relative_path_to_texture,
																			   const unsigned int width_subTexture,
																			   const unsigned int height_subtexture)
	{
		std::shared_ptr<RenderEngine::Texture2D> texture = loadTexture(texture_name, relative_path_to_texture);
		if (texture == nullptr)
		{
			std::cerr << "Can't load texture atlas: " << relative_path_to_texture << std::endl;
			return nullptr;
		}

		const unsigned int texture_width = texture->width();
		const unsigned int texture_height = texture->height();

		unsigned int current_position_u = 0;
		unsigned int current_position_v = texture_height;

		for (const std::string& name : subTexture_names)
		{
			glm::vec2 left_bottom_uv(static_cast<float>(current_position_u) / texture_width,
									 static_cast<float>(current_position_v - height_subtexture) / texture_height);

			glm::vec2 right_top_uv(static_cast<float>(current_position_u + width_subTexture) / texture_width,
								   static_cast<float>(current_position_v) / texture_height);

			texture->addSubTexture(name, left_bottom_uv, right_top_uv);

			current_position_u += width_subTexture;

			if (current_position_u >= texture_width)
			{
				current_position_v -= height_subtexture;
				current_position_u = 0;
			}
		}
		return texture;
	}



	bool ResourceManager::loadResourcesJSON(const std::string& path_to_JSON_file)
	{
		const std::string JSON_string = ResourceManager::getFileString(m_path + "/" + path_to_JSON_file);
		if (JSON_string.empty())
		{
			std::cerr << "JSON file is empty: " << path_to_JSON_file << std::endl;
			return false;
		}

		rapidjson::Document document;
		rapidjson::ParseResult parse_result = document.Parse(JSON_string.c_str());
		if (!parse_result)
		{
			std::cerr << "Parse error JSON file: " << rapidjson::GetParseError_En(parse_result.Code()) << "(" << parse_result.Offset() << ")" << std::endl;
			std::cerr << "JSON file: " << path_to_JSON_file << std::endl;
			return false;
		}

		if (!ResourceManager::loadShaderProgramsJSON(document)) { return false; };
		if (!ResourceManager::loadTextureAtlasesJSON(document)) { return false; };
		if (!ResourceManager::loadAnimatedSpritesJSON(document)) { return false; };

		return true;
	}



	// PRIVATE
	std::string ResourceManager::getFileString(const std::string& path)
	{
		std::ifstream file(path, std::ios::in | std::ios::binary);
		if (!file.is_open())
		{
			std::cerr << "Can't load data from file: " << path << std::endl;
			return std::string{};
		}
		
		std::stringstream buffer;
		buffer << file.rdbuf();
		return buffer.str();
	}



	bool ResourceManager::loadShaderProgramsJSON(const rapidjson::Document& document)
	{
		auto shader_programs_iterator = document.FindMember("shaders");
		if (shader_programs_iterator != document.MemberEnd())
		{
			for (const auto& current_shader_program : shader_programs_iterator->value.GetArray())
			{
				std::string shader_program_name = current_shader_program["name"].GetString();
				std::string vertex_shader_path = current_shader_program["filePath_v"].GetString();
				std::string fragment_shader_path = current_shader_program["filePath_f"].GetString();
				auto shader_program = loadShaderProrgam(std::move(shader_program_name), std::move(vertex_shader_path),
														std::move(fragment_shader_path));
				if (shader_program == nullptr)
				{
					std::cerr << "Can't load shader program from JSON" << std::endl;
					return false;
				}
			}
		}
		return true;
	}



	bool ResourceManager::loadTextureAtlasesJSON(const rapidjson::Document& document)
	{
		auto texture_atlases_iterator = document.FindMember("textureAtlases");
		if (texture_atlases_iterator != document.MemberEnd())
		{
			for (const auto& current_texture_atlass : texture_atlases_iterator->value.GetArray())
			{
				std::string texture_atlas_name = current_texture_atlass["name"].GetString();
				std::string texture_atlas_path = current_texture_atlass["filePath"].GetString();
				unsigned int subTexture_width = current_texture_atlass["subTextureWidth"].GetUint();
				unsigned int subTexture_height = current_texture_atlass["subTextureHeight"].GetUint();

				auto subTextures = current_texture_atlass["subTextureNames"].GetArray();
				std::vector<std::string> subTexture_names;
				subTexture_names.reserve(subTextures.Size());

				for (const auto& current_name : subTextures)
				{
					subTexture_names.emplace_back(current_name.GetString());
				}

				auto texture_atlas = loadTextureAtlas(std::move(texture_atlas_name), std::move(subTexture_names),
													  std::move(texture_atlas_path), std::move(subTexture_width), 
													  std::move(subTexture_height));
				if (texture_atlas == nullptr)
				{
					std::cerr << "Can't load texture atlas from JSON" << std::endl;
					return false;
				}
			}
		}
		return true;
	}



	bool ResourceManager::loadAnimatedSpritesJSON(const rapidjson::Document& document)
	{
		auto animated_sprites_iterator = document.FindMember("animatedSprites");
		if (animated_sprites_iterator != document.MemberEnd())
		{
			for (const auto& current_animated_sprite : animated_sprites_iterator->value.GetArray())
			{
				std::string animated_sprite_name = current_animated_sprite["name"].GetString();
				std::string animated_sprite_texture_atlas = current_animated_sprite["textureAtlas"].GetString();
				std::string animated_sprite_shader_program = current_animated_sprite["shaderProgram"].GetString();
				unsigned int animated_sprite_width = current_animated_sprite["initalWidth"].GetUint();
				unsigned int animated_sprite_height = current_animated_sprite["initalHeight"].GetUint();
				std::string animated_sprite_inital_subTexture = current_animated_sprite["initalSubTexture"].GetString();

				std::shared_ptr<RenderEngine::AnimatedSprite2D> sprite = loadAnimatedSprite(std::move(animated_sprite_name), std::move(animated_sprite_shader_program),
																							std::move(animated_sprite_texture_atlas), std::move(animated_sprite_width),
																							std::move(animated_sprite_height), std::move(animated_sprite_inital_subTexture));
				if (sprite == nullptr)
				{
					std::cerr << "Can't load animated sprite from JSON" << std::endl;
					return false;
				}

				auto rapid_states = current_animated_sprite["states"].GetArray();
				for (const auto& state : rapid_states)
				{
					std::string state_name = state["stateName"].GetString();
					auto rapid_state_frames = state["frames"].GetArray();

					std::vector<std::pair<std::string, uint64_t>> state_frames;
					state_frames.reserve(rapid_state_frames.Size());

					for (const auto& frame : rapid_state_frames)
					{
						state_frames.emplace_back(std::make_pair(frame["subTexture"].GetString(), frame["duration"].GetUint()));
					}

					sprite->addState(std::move(state_name), std::move(state_frames));
				}
			}
		}
		return true;
	}
}