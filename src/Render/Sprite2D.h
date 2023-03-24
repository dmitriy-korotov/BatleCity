#pragma once
#ifndef SPRITE2D_H
#define SPRITE2D_H

#include <string>
#include <vector>
#include <map>
#include <glm/vec2.hpp>
#include <glad/glad.h>

#include "IRenderObjectRect2D.h"

namespace RenderEngine
{
	class Texture2D;
	class SpriteAnimator;

	class Sprite2D : IRenderObjectRect2D
	{

		friend SpriteAnimator;

	public:
		Sprite2D(std::shared_ptr<Texture2D> ptr_texture,
				 std::shared_ptr<ShaderProgram> ptr_shader_program,
				 std::string subTexture_name = "default");
		~Sprite2D();

		Sprite2D(const Sprite2D&) = delete;
		Sprite2D(Sprite2D&&) = delete;
		Sprite2D& operator=(const Sprite2D&) = delete;
		Sprite2D& operator=(Sprite2D&&) = delete;

		bool isAnimated() const;
		void render(const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer) const override;
		void addState(const std::string& state_name, std::vector<std::pair<std::string, double>>& frames);

	protected:

		double getDurationStateAnimation(const std::string& state_name) const;

		using MapStates = std::map<std::string, std::vector<std::pair<std::string, double>>>;

		MapStates m_states;

		std::shared_ptr<Texture2D> m_texture;
		VertexBuffer m_texture_coords_buffer;
	};
}

#endif