#include "scene/aabb.h"

namespace gore
{
	aabb::aabb() : center_{0.0f}, extend_{0.0f}
	{
	}
	aabb::aabb(glm::vec3 min, glm::vec3 max) : center_{ (max - min) * 0.5f }, extend_{ max - center_ }
	{
		
	}
}