#pragma once
#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <rapidjson/document.h>

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace BatleCity {
class IGameState;
class StartScreen;
class Level;
}  // namespace BatleCity

namespace RenderEngine {
class ShaderProgram;
class Texture2D;
class Sprite2D;
class subTexture2D;
class AnimatedSprite2D;
}  // namespace RenderEngine

namespace Resources {
class ResourceManager final {
 public:
  void SetExecutablePath(const std::string& executable_path);
  void UnloadAllResources();

  std::shared_ptr<RenderEngine::ShaderProgram> LoadShaderProrgam(
      const std::string& shader_name,
      const std::string& path_to_vertex_shader_source,
      const std::string& path_to_fragment_shader_source);
  std::shared_ptr<RenderEngine::ShaderProgram> GetShaderProgram(
      const std::string& shader_name);

  std::shared_ptr<RenderEngine::Texture2D> LoadTexture(
      const std::string& texture_name,
      const std::string& relative_path_to_texture);
  std::shared_ptr<RenderEngine::Texture2D> GetTexture(
      const std::string& texture_name);

  std::shared_ptr<RenderEngine::Sprite2D> LoadSprite(
      const std::string& sprite_name, const std::string& shader_program_name,
      const std::string& texture_name,
      const std::string& subTexture_name = "default");
  std::shared_ptr<RenderEngine::Sprite2D> GetSprite(
      const std::string& sprite_name);

  std::shared_ptr<RenderEngine::Texture2D> LoadTextureAtlas(
      const std::string& texture_name,
      const std::vector<std::string> subTexture_names,
      const std::string& relative_path_to_texture,
      const unsigned int width_subTexture,
      const unsigned int height_subtexture);

  std::shared_ptr<BatleCity::IGameState> LoadLevel(
      std::string&& level_name, std::vector<std::string>&& level_description);
  std::shared_ptr<BatleCity::IGameState> LoadStartScreen(
      std::string&& start_screeen_name,
      std::vector<std::string>&& start_screen_description,
      unsigned int left_offset, unsigned int bottom_offset,
      unsigned int menu_position_x, unsigned int menu_position_y);
  std::shared_ptr<BatleCity::StartScreen> GetStartScreen(
      const std::string& start_screen_name);
  std::shared_ptr<BatleCity::Level> GetLevel(const std::string& level_name);

  bool LoadAllResourcesJSON(const std::string& path_to_JSON_file);

 private:
  std::string GetFileString(const std::string& path);
  bool LoadShaderProgramsJSON(const rapidjson::Document& document);
  bool LoadTextureAtlasesJSON(const rapidjson::Document& document);
  bool LoadSpritesJSON(const rapidjson::Document& document);
  bool LoadLevelsJSON(const rapidjson::Document& document);
  bool LoadStartScreensJSON(const rapidjson::Document& document);

  typedef std::map<std::string, std::shared_ptr<RenderEngine::ShaderProgram>>
      MapShaderProgram;
  MapShaderProgram m_shader_programs;

  typedef std::map<std::string, std::shared_ptr<RenderEngine::Texture2D>>
      MapTexture2D;
  MapTexture2D m_textures;

  typedef std::map<std::string, std::shared_ptr<RenderEngine::Sprite2D>>
      MapSprite2D;
  MapSprite2D m_sprites;

  typedef std::map<std::string, std::shared_ptr<BatleCity::IGameState>>
      MapGameStates;
  MapGameStates m_game_states;

  std::string m_path;
};
}  // namespace Resources

#endif  // !RESOURCE_MANAGER_H
