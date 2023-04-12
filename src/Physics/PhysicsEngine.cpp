#include "PhysicsEngine.h"

#include <algorithm>
#include <iostream>
#include <memory>



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
		std::vector<std::shared_ptr<BatleCity::IDynamicGameObject>> destroyed_game_objects;
		destroyed_game_objects.reserve(m_dynamic_game_objects.size());

		for (auto& dynamic_game_object : m_dynamic_game_objects)
		{
			if (dynamic_game_object->isDestroy())
			{
				destroyed_game_objects.emplace_back(dynamic_game_object);
			}
			else
			{
				if (dynamic_game_object->getVelocity() > 0)
				{
					const glm::vec2 new_position = getNewPosition(dynamic_game_object, delta);
					auto objects = m_current_level->getObjectsFromArea(new_position, dynamic_game_object->getSize());
					bool is_intersection = isInersectionWithObjects(dynamic_game_object, new_position, objects);

					if (!is_intersection)
					{
						dynamic_game_object->setPosition(new_position);
					}

					/*for (auto& other_game_object : m_dynamic_game_objects)
					{
						if (isIntersection(game_object->getColliders(), game_object->getPosition(), other_game_object->getColliders(), other_game_object->getPosition()))
						{
							game_object->onCollision(other_game_object->getGameObjectType());
						}
					}*/
				}
			}
		}
		
		if (!destroyed_game_objects.empty())
		{
			for (const auto& destroyed_game_object : destroyed_game_objects)
			{
				m_dynamic_game_objects.erase(destroyed_game_object);
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



	glm::vec2 PhysicsEngine::getNewPosition(const std::shared_ptr<BatleCity::IDynamicGameObject>& game_object, double delta)
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
		return new_position;
	}



	std::pair<std::shared_ptr<AABB>, std::shared_ptr<AABB>> PhysicsEngine::isIntersection(const std::vector<AABB>& first_object, const glm::vec2& position_object1,
																						  const std::vector<AABB>& second_object, const glm::vec2& position_object2)
	{
		for (const auto& first_collision : first_object)
		{
			for (const auto& second_collision : second_object)
			{
				if (first_collision.isActive() && second_collision.isActive())
				{
					if (first_collision.getLeftBottom().x + position_object1.x >= second_collision.getRightTop().x + position_object2.x)
					{
						continue;
					}
					if (first_collision.getRightTop().x + position_object1.x <= second_collision.getLeftBottom().x + position_object2.x)
					{
						continue;
					}
					if (first_collision.getLeftBottom().y + position_object1.y >= second_collision.getRightTop().y + position_object2.y)
					{
						continue;
					}
					if (first_collision.getRightTop().y + position_object1.y <= second_collision.getLeftBottom().y + position_object2.y)
					{
						continue;
					}
					return std::make_pair<std::shared_ptr<AABB>, std::shared_ptr<AABB>>(std::make_shared<AABB>(first_collision), std::make_shared<AABB>(second_collision));
				}
			}
		}
		return std::make_pair<std::shared_ptr<AABB>, std::shared_ptr<AABB>>(nullptr, nullptr);
	}



	bool PhysicsEngine::isInersectionWithObjects(const std::shared_ptr<BatleCity::IDynamicGameObject>& current_game_object,
												 const glm::vec2& new_position,
												 const std::vector<std::shared_ptr<BatleCity::IGameObject>>& other_objects)
	{
		for (const auto& object : other_objects)
		{
			auto colliders_pair = isIntersection(current_game_object->getColliders(), new_position, object->getColliders(), object->getPosition());
			if (colliders_pair.first != nullptr && colliders_pair.second != nullptr)
			{
				const bool is_stoped = current_game_object->onCollision(object->getGameObjectType(), colliders_pair.first);
				object->onCollision(current_game_object->getGameObjectType(), colliders_pair.second, current_game_object->getDirection());
				if (is_stoped)		return true;
			}
		}
		return false;
	}
}