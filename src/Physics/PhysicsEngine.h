#pragma once
#ifndef PHYSICS_ENGINE_H
#define PHYSICS_ENGINE_H

#include <unordered_set>
#include <memory>

#include "../Game/GameObjects/IDynamicGameObject.h"
#include "../Game/GameObjects/IGameObject.h"

namespace Physics
{
	class PhysicsEngine
	{
	public:

		PhysicsEngine() = delete;
		PhysicsEngine(const PhysicsEngine&) = delete;
		PhysicsEngine& operator=(const PhysicsEngine&) = delete;
		PhysicsEngine(PhysicsEngine&&) = delete;
		PhysicsEngine& operator=(PhysicsEngine&&) = delete;
		~PhysicsEngine() = delete;

		static void init();
		static void terminate();
		static void update(double delta);
		static void addDynamicGameObject(std::shared_ptr<BatleCity::IDynamicGameObject> dynamic_game_object);

	private:

		static std::unordered_set<std::shared_ptr<BatleCity::IDynamicGameObject>> m_dynamic_game_objects;

	};
}

#endif // PHYSICS_ENGINE_H
