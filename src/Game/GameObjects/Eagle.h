#pragma once
#ifndef EAGLE_H
#define EAGLE_H

#include <memory>
#include <array>

#include "IGameObject.h"

#include "../../Render/Sprite2D.h"

namespace BatleCity
{
	class Eagle : public IGameObject
	{
	public:

		enum class EEagleState
		{
			Elive = 0,
			Dead
		};

		Eagle(const glm::vec2& positiion, const glm::vec2& size, const float rotation, const float layer);

		void render() const override;
		void update(const uint64_t delta) override;

	private:

		EEagleState m_eagle_state;
		const std::array<std::shared_ptr<RenderEngine::Sprite2D>, 2> m_eagle_sprites;

	};
}

#endif // !EAGLE_H
