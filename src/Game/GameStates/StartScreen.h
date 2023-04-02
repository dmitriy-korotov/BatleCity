#pragma once
#ifndef START_SCREEN_H
#define START_SCREEN_H

#include "IGameState.h"

#include "../GameObjects/IGameObject.h"

#include <vector>

namespace BatleCity
{
	class StartScreen : public IGameState
	{
	public:

		StartScreen(const std::vector<std::string>& start_screen_description);
		~StartScreen() { };

		size_t getGameStateWidth() const noexcept override;
		size_t getGameStateHeight() const noexcept override;

		bool start() const noexcept override;
		void render() const override;
		void update(double delta, const std::array<bool, 349>& keyboard) override;

	private:

		std::vector<std::shared_ptr<IGameObject>> m_start_screen_elements;

	};
}

#endif // !START_SCREEN_H
