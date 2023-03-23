#pragma once
#ifndef AXIS_ALIGNED_BOUNDING_BOX
#define AXIS_ALIGNED_BOUNDING_BOX

#include <glm/vec2.hpp>
#include <glad/glad.h>

#include "../Render/VertexBuffer.h"
#include "../Render/VertexArray.h"
#include "../Render/ElementBuffer.h"

namespace Physics
{
	struct AxisAlignedBoundingBox
	{
	public:

		AxisAlignedBoundingBox() = delete;
		AxisAlignedBoundingBox(const glm::vec2& _left_bottom, const glm::vec2& _right_top);
		~AxisAlignedBoundingBox();

		inline const glm::vec2& getLeftBottom() const noexcept { return left_bottom; }
		inline const glm::vec2& getRightTop() const noexcept { return right_top; }

		void render(const glm::vec2& position, float rotation) const;

	private:

		glm::vec2 left_bottom;
		glm::vec2 right_top;

		//RenderEngine::VertexArray VAO;
		//RenderEngine::ElementBuffer element_buffer;

		GLuint m_VAO;
		GLuint m_element_buffer;
		GLuint m_coords_buffer;
		GLuint m_colors_buffer;
		GLuint m_data_buffer;

	};
	typedef AxisAlignedBoundingBox AABB;
}

#endif // !AXIS_ALIGNED_BOUNDING_BOX
