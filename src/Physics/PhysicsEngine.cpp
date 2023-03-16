#include "PhysicsEngine.h"

namespace Physics
{
	std::unordered_set<std::shared_ptr<BatleCity::IDynamicGameObject>> PhysicsEngine::m_dynamic_game_objects;



	void PhysicsEngine::init()
	{

	}



	void PhysicsEngine::terminate()
	{
		m_dynamic_game_objects.clear();
	}



	void PhysicsEngine::update(double delta)
	{
		for (auto& game_object : m_dynamic_game_objects)
		{
			if (game_object->getVelocity() > 0)
			{
				game_object->move(game_object->getDirection() * static_cast<float>(game_object->getVelocity() * delta));
			}
		}
	}



	void PhysicsEngine::addDynamicGameObject(std::shared_ptr<BatleCity::IDynamicGameObject> dynamic_game_object)
	{
		m_dynamic_game_objects.insert(std::move(dynamic_game_object));
	}
}