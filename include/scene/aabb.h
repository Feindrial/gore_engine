#ifndef GORE_AABB_H
#define GORE_AABB_H

#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/common.hpp"
#include "glm/gtx/component_wise.hpp"

#include <array>
#include <cmath>

namespace gore
{
	struct aabb
	{
		enum class intersection_type
		{
			inside,
			cross,
			outside
		};

		aabb();
		aabb(glm::vec3 min, glm::vec3 max);
		void try_to_extend(glm::vec3 &min, glm::vec3 &max);

		std::pair<std::array<glm::vec3, 8>, std::array<unsigned int, 16>> get_cube() const;
		bool intersect(const aabb& ab) const;

		glm::vec3 center_;
		glm::vec3 extend_;
	};

	inline void aabb::try_to_extend(glm::vec3 &min, glm::vec3 &max)
	{
		center_ = (max + min) * 0.5f;
		extend_ = max - center_;
	}

	inline std::pair<std::array<glm::vec3, 8>, std::array<unsigned int, 16>> aabb::get_cube() const
	{
		std::array<glm::vec3, 8> points{
			glm::vec3(center_.x - extend_.x, center_.y - extend_.y, center_.z - extend_.z),
			glm::vec3(center_.x + extend_.x, center_.y - extend_.y, center_.z - extend_.z),
			glm::vec3(center_.x + extend_.x, center_.y - extend_.y, center_.z + extend_.z),
			glm::vec3(center_.x - extend_.x, center_.y - extend_.y, center_.z + extend_.z),
			glm::vec3(center_.x - extend_.x, center_.y + extend_.y, center_.z - extend_.z),
			glm::vec3(center_.x + extend_.x, center_.y + extend_.y, center_.z - extend_.z),
			glm::vec3(center_.x + extend_.x, center_.y + extend_.y, center_.z + extend_.z),
			glm::vec3(center_.x - extend_.x, center_.y + extend_.y, center_.z + extend_.z),
		};

		std::array<unsigned int, 16> lines{
			0, 1, 2, 3, 0, 4, 7, 3,
			6, 7, 4, 5, 6, 2, 1, 5
		};

		return std::make_pair(points, lines);
	}
	inline bool aabb::intersect(const aabb &ab) const
	{
  		if (std::fabs(center_.x - ab.center_.x) > (extend_.x + ab.extend_.x)) return false;
		if (std::fabs(center_.y - ab.center_.y) > (extend_.y + ab.extend_.y)) return false;
		if (std::fabs(center_.z - ab.center_.z) > (extend_.z + ab.extend_.z)) return false;
		
		return true;
	}
}

#endif