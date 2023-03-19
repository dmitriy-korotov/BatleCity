#include "PhysicsEngine.h"

namespace Physics
{
	std::unordered_set<std::shared_ptr<BatleCity::IDynamicGameObject>> PhysicsEngine::m_dynamic_game_objects;
	std::shared_ptr<BatleCity::Level> PhysicsEngine::m_current_level;



	void PhysicsEngine::init()
	{

	}



	void PhysicsEngine::terminate()
	{
		m_dynamic_game_objects.clear();
		m_current_level.reset();		
	}



	void PhysicsEngine::update(double delta)
	{
		for (auto& game_object : m_dynamic_game_objects)
		{
			if (game_object->getVelocity() > 0)
			{
			    const glm::vec2 game_object_direction = game_object->getDirection();
				glm::vec2 new_position(0.f);

				if (game_object_direction.x == 0.f)
				{
					new_position.x = static_cast<float>(static_cast<int>(game_object->getPosition().x / 4.f + 0.5f) * 4);
					new_position.y = game_object->getPosition().y + game_object_direction.y * static_cast<float>(game_object->getVelocity() * delta);
				}
				else
				{
					new_position.y = static_cast<float>(static_cast<int>(game_object->getPosition().y / 4.f + 0.5f) * 4);
					new_position.x = game_object->getPosition().x + game_object_direction.x * static_cast<float>(game_object->getVelocity() * delta);
				}
				
				auto objects = m_current_level->getObjectsFromArea(new_position, game_object->getSize());

				bool is_intersection = false;
				for (const auto& object : objects)
				{
					if (isIntersection(object->getColliders(), object->getPosition(), game_object->getColliders(), new_position))
					{
						game_object->onCollision();
						is_intersection = true;
						break;
					}
				}
				
				if (!is_intersection)
				{
					game_object->setPosition(new_position);
				}
			}
		}
	}


	void PhysicsEngine::setCurrentLevel(std::shared_ptr<BatleCity::Level> current_level)
	{
		m_current_level.swap(current_level);
	}



	void PhysicsEngine::addDynamicGameObject(std::shared_ptr<BatleCity::IDynamicGameObject> dynamic_game_object)
	{
		m_dynamic_game_objects.insert(std::move(dynamic_game_object));
	}



	bool PhysicsEngine::isIntersection(const std::vector<AABB>& first_object, const glm::vec2& position_object1,
									   const std::vector<AABB>& second_object, const glm::vec2& position_object2)
	{
		for (const auto& first_collision : first_object)
		{
			for (const auto& second_collision : second_object)
			{
				if (first_collision.left_bottom.x + position_object1.x >= second_collision.right_top.x + position_object2.x)
				{
					continue;
				}
				if (first_collision.right_top.x + position_object1.x <= second_collision.left_bottom.x + position_object2.x)
				{
					continue;
				}
				if (first_collision.left_bottom.y + position_object1.y >= second_collision.right_top.y + position_object2.y)
				{
					continue;
				}
				if (first_collision.right_top.y + position_object1.y <= second_collision.left_bottom.y + position_object2.y)
				{
					continue;
				}
				return true;
			}
		}
		return false;
	}
}