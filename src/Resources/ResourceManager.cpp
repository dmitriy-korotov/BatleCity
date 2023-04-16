#include "ResourceManager.h"

#include "../Render/ShaderProgram.h"
#include "../Render/Texture2D.h"
#include "../Render/Sprite2D.h"
#include "../Render/AnimatedSprite2D.h"

#include "../Game/GameStates/IGameState.h"
#include "../Game/GameStates/Level.h"
#include "../Game/GameStates/StartScreen.h"

#include <iostream>
#include <fstream>
#include <sstream>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "stb_image.h"
#include "../rapidjson/error/en.h"



namespace Resources
{
	ResourceManager::MapShaderProgram ResourceManager::m_shader_programs;
	ResourceManager::MapTexture2D ResourceManager::m_textures;
	ResourceManager::MapSprite2D ResourceManager::m_sprites;
	ResourceManager::MapGameStates ResourceManager::m_game_states;
	std::string ResourceManager::m_path;



	void ResourceManager::setExecutablePath(const std::string& executable_path)
	{
		size_t finded_index = executable_path.find_last_of("/\\");

		m_path = executable_path.substr(0, finded_index);
	}



	void ResourceManager::unloadAllResources()
	{
		m_shader_programs.clear();
		m_sprites.clear();
		m_textures.clear();
		m_game_states.clear();
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
																					   subTexture_name)).first->second;
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



	std::shared_ptr<BatleCity::IGameState> ResourceManager::loadLevel(std::string& level_name, std::vector<std::string>& level_description)
	{
		if (level_description.empty())
		{
			std::cerr << "Level description is empty can't load level" << std::endl;
			return nullptr;
		}
		return m_game_states.emplace(std::move(level_name), std::make_shared<BatleCity::Level>(std::move(level_description))).first->second;
	}



	std::shared_ptr<BatleCity::IGameState> ResourceManager::loadStartScreen(std::string& start_screen_name, std::vector<std::string>& start_screen_description,
																			unsigned int left_offset, unsigned int bottom_offset,
																			unsigned int menu_position_x, unsigned int menu_position_y)
	{
		if (start_screen_description.empty())
		{
			std::cerr << "Start screen description is empty can't load start screen" << std::endl;
			return nullptr;
		}
		return m_game_states.emplace(std::move(start_screen_name), std::move(std::make_shared<BatleCity::StartScreen>(std::move(start_screen_description),
																													  left_offset,
																													  bottom_offset,
																													  menu_position_x,
																													  menu_position_y))).first->second;
	}



	std::shared_ptr<BatleCity::StartScreen> ResourceManager::getStartScreen(const std::string& start_sccreen_name)
	{
		const auto& game_state = m_game_states.find(start_sccreen_name);
		if (game_state == m_game_states.end())
		{
			std::cerr << "ERROR: Game state with such name not found: name = " << start_sccreen_name << std::endl;
			return nullptr;
		}
		
		BatleCity::IGameState* game_state_ptr = &(*game_state->second);

		if (!dynamic_cast<BatleCity::StartScreen*>(game_state_ptr))
		{
			std::cerr << "ERROR: This game state is not start screen: name = " << start_sccreen_name << std::endl;
			return nullptr;
		}

		return reinterpret_cast<std::shared_ptr<BatleCity::StartScreen>&>(game_state->second);
	}



	std::shared_ptr<BatleCity::Level> ResourceManager::getLevel(const std::string& level_name)
	{
		const auto& game_state = m_game_states.find(level_name);
		if (game_state == m_game_states.end())
		{
			std::cerr << "ERROR: Game state with such name not found: name = " << level_name << std::endl;
			return nullptr;
		}

		BatleCity::IGameState* game_state_ptr = &(*game_state->second);

		if (!dynamic_cast<BatleCity::Level*>(game_state_ptr))
		{
			std::cerr << "ERROR: this game state is not level: name = " << level_name << std::endl;
			return nullptr;
		}

		return reinterpret_cast<std::shared_ptr<BatleCity::Level>&>(game_state->second);
	}



	bool ResourceManager::loadAllResourcesJSON(const std::string& path_to_JSON_file)
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

		if (!ResourceManager::loadShaderProgramsJSON(document)) { return false; }
		if (!ResourceManager::loadTextureAtlasesJSON(document)) { return false; }
		if (!ResourceManager::loadSpritesJSON(document)) { return false; }
		if (!ResourceManager::loadStartScreensJSON(document)) { return false; }
		if (!ResourceManager::loadLevelsJSON(document)) { return false; }

		return true;
	}



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




	bool ResourceManager::loadSpritesJSON(const rapidjson::Document& document)
	{
		auto sprites_iterator = document.FindMember("sprites");
		if (sprites_iterator != document.MemberEnd())
		{
			for (const auto& current_sprite : sprites_iterator->value.GetArray())
			{
				std::string sprite_name = current_sprite["name"].GetString();
				std::string texture_atlas_name = current_sprite["textureAtlas"].GetString();
				std::string shader_program_name = current_sprite["shaderProgram"].GetString();
				std::string subTexture_name = current_sprite["subTexture"].GetString();

				auto sprite = loadSprite(sprite_name, std::move(shader_program_name), std::move(texture_atlas_name), std::move(subTexture_name));
				if (!sprite)
				{
					std::cerr << "Can't load sprite from JSON: " << sprite_name << std::endl;
					return false;
				}

				auto rapid_states = current_sprite.FindMember("states");
				if (rapid_states != current_sprite.MemberEnd())
				{
					for (const auto& state : rapid_states->value.GetArray())
					{
						std::string state_name = state["stateName"].GetString();
						auto rapid_state_frames = state["frames"].GetArray();

						std::vector<std::pair<std::string, double>> state_frames;
						state_frames.reserve(rapid_state_frames.Size());

						for (const auto& frame : rapid_state_frames)
						{
							state_frames.emplace_back(std::make_pair(frame["sprite"].GetString(), frame["duration"].GetDouble()));
						}

						sprite->addState(std::move(state_name), std::move(state_frames));
					}
				}
			}
		}
		return true;
	}



	bool ResourceManager::loadStartScreensJSON(const rapidjson::Document& document)
	{
		auto start_screen_iterator = document.FindMember("start_screens");
		if (start_screen_iterator != document.MemberEnd())
		{
			for (const auto& start_screen : start_screen_iterator->value.GetArray())
			{
				std::string type_game_state = start_screen["type"].GetString();
				std::string name_start_screen = start_screen["name"].GetString();
				uint16_t left_offset = start_screen["left_offset"].GetUint();
				uint16_t bottom_offset = start_screen["bottom_offset"].GetUint();
				uint16_t menu_position_x = start_screen["menu_position_x"].GetUint();
				uint16_t menu_position_y = start_screen["menu_position_y"].GetUint();
				auto start_screen_description = start_screen["description"].GetArray();

				std::vector<std::string> description;
				description.reserve(start_screen_description.Size());

				unsigned int max_length = 0;
				for (const auto& row : start_screen_description)
				{
					std::string row_start_screen = row.GetString();
					max_length = max_length < row_start_screen.length() ? row_start_screen.length() : max_length;
					description.emplace_back(std::move(row_start_screen));
				}

				for (std::string& row : description)
				{
					while (max_length > row.length())
					{
						row.append("F");
					}
				}
				loadStartScreen(std::move(name_start_screen), std::move(description), left_offset, bottom_offset, menu_position_x, menu_position_y);
			}
		}
		return true;
	}



	bool ResourceManager::loadLevelsJSON(const rapidjson::Document& document)
	{
		auto levels_iterator = document.FindMember("levels");
		if (levels_iterator != document.MemberEnd())
		{
			for (const auto& level : levels_iterator->value.GetArray())
			{
				std::string type_game_state = level["type"].GetString();
				std::string name_level = level["name"].GetString();
				auto level_description = level["description"].GetArray();

				std::vector<std::string> description;
				description.reserve(level_description.Size());

				unsigned int max_length = 0;
				for (const auto& row : level_description)
				{
					std::string row_level = row.GetString();
					max_length = max_length < row_level.length() ? row_level.length() : max_length;
					description.emplace_back(std::move(row_level));
				}

				for (std::string& row : description)
				{
					while (max_length > row.length())
					{
						row.append("D");
					}
				}
				loadLevel(std::move(name_level), std::move(description));				
			}
		}
		return true;
	}
}