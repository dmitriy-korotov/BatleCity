#pragma once
#ifndef LEVEL_H
#define LEVEL_H

#include <string>
#include <vector>
#include <memory>

#include "GameObjects/IGameObject.h"

namespace BatleCity
{
	class IGameObject;

	class Level
	{
	public:

		Level(const std::vector<std::string>& level_description);

		Level(const Level&) = delete;
		Level& operator=(const Level&) = delete;

		Level(Level&& other) noexcept;
		Level& operator=(Level&& _right) noexcept;

		size_t getLevelWidth() const;
		size_t getLevelHeight() const;

		void update(const uint64_t delta);
		void render() const;

	private:

		unsigned int m_width = 0;
		unsigned int m_height = 0;

		std::vector<std::shared_ptr<IGameObject>> m_map_objects;

	};
}

#endif // !LEVEL_H
