#include "IGameObject.h"
#ifndef I_DYNAMIC_GAME_OBJECT
#define I_DYNAMIC_GAME_OBJECT

namespace BatleCity
{
	class IDynamicGameObject : public IGameObject
	{
	public:


		enum class EOrientation
		{
			Top = 0,
			Right,
			Bottom,
			Left
		};


		IDynamicGameObject() = default;
		IDynamicGameObject(EGameObjectType game_object_type, const glm::vec2& position, const glm::vec2& size,
						   float rotation, float layer, const glm::vec2& direction, double velocity, double max_velocity);

		void move(const glm::vec2& offset);
		virtual void setVelocity(double velocity);
		void setDirection(const glm::vec2& direction);
		virtual void setOrientation(const EOrientation orientation);

		double getMaxVelocity() const;
		double getVelocity() const;
		const glm::vec2& getDirection() const;
		EOrientation getOrientation() const noexcept;

		inline bool isDestroy() const noexcept { return m_is_destroy; };

	protected:

		EOrientation m_current_orientation = EOrientation::Top;
		glm::vec2 m_direction = glm::vec2(0.f, 1.f);
		double m_max_velocity = 0;
		double m_velocity = 0;
		bool m_is_destroy = false;

	};
}

#endif // !I_DYNAMIC_GAME_OBJECT

