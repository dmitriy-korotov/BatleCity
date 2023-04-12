#pragma once
#ifndef PHYSICS_ENGINE_H
#define PHYSICS_ENGINE_H

#include <vector>
#include <unordered_set>
#include <memory>

#include "../Game/GameObjects/IGameObject.h"
#include "../Game/GameObjects/IDynamicGameObject.h"
#include "../Game/GameStates/Level.h"

#include "AxisAlignedBoundingBox.h"

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
		static void setCurrentLevel(std::shared_ptr<BatleCity::Level> current_level);

	private:

		static glm::vec2 getNewPosition(const std::shared_ptr<BatleCity::IDynamicGameObject>& game_object, double delta);

		static std::pair<std::shared_ptr<AABB>, std::shared_ptr<AABB>> isIntersection(const std::vector<AABB>& first_object, const glm::vec2& position_object1,
																					  const std::vector<AABB>& second_object, const glm::vec2& position_object2);

		static bool isInersectionWithObjects(const std::shared_ptr<BatleCity::IDynamicGameObject>& current_game_object,
											 const glm::vec2& new_position,
											 const std::vector<std::shared_ptr<BatleCity::IGameObject>>& other_objects);

		static std::unordered_set<std::shared_ptr<BatleCity::IDynamicGameObject>> m_dynamic_game_objects;
		static std::shared_ptr<BatleCity::Level> m_current_level;

	};
}

#endif // PHYSICS_ENGINE_H
