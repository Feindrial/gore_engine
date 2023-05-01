#include "util/util.h"

#include "glad/glad.h"
#include "glm/mat4x4.hpp"

#include <GL/gl.h>
#include <string_view>

namespace gore
{
	namespace util
	{
		float interpolate(float min, float max, float value, interpolation_type type)
		{
			//assert(value >= Min && value <= Max);
			
		

			if ((min - value) * -1.0f > max - value)
				return value / max ; 
			else
				return value / max;
		}

		std::pair<std::array<glm::vec3, 8>, std::array<unsigned int, 16>> get_cube(glm::vec3 center, glm::vec3 extend)
		{
			std::array<glm::vec3, 8> points{
				glm::vec3(center.x - extend.x, center.y - extend.y, center.z - extend.z),  // <- +x | ^ +y | (*) +z
				glm::vec3(center.x + extend.x, center.y - extend.y, center.z - extend.z),  //    4-----5	
				glm::vec3(center.x + extend.x, center.y - extend.y, center.z + extend.z),  //	/|    /|
 				glm::vec3(center.x - extend.x, center.y - extend.y, center.z + extend.z),  //  7-----6 |
				glm::vec3(center.x - extend.x, center.y + extend.y, center.z - extend.z),  //  | |   | |
				glm::vec3(center.x + extend.x, center.y + extend.y, center.z - extend.z),  //  | 0-----1
				glm::vec3(center.x + extend.x, center.y + extend.y, center.z + extend.z),  //  |/	 |/
				glm::vec3(center.x - extend.x, center.y + extend.y, center.z + extend.z),  //  3-----2
			};

			constexpr std::array<unsigned int, 16> lines{
				0, 1, 2, 3, 0, 4, 7, 3,
				6, 7, 4, 5, 6, 2, 1, 5
			};

			return std::make_pair(points, lines);
		}
		std::pair<std::array<glm::vec3, 8>, std::array<unsigned int, 16>> get_cube(glm::vec3 size, glm::mat4 matrix)
		{
			std::array<glm::vec3, 8> points{
				glm::vec3(size.x, -size.y, -size.z),   // <- +x | ^ +y | (*) +z
				glm::vec3(-size.x, -size.y, -size.z),  //    4-----5	
				glm::vec3(-size.x, -size.y, size.z),   //	/|    /|
 				glm::vec3(size.x, -size.y, size.z),    //  7-----6 |
				glm::vec3(size.x, size.y, -size.z),    //  | |   | |
				glm::vec3(-size.x, size.y, -size.z),   //  | 0-----1
				glm::vec3(-size.x, size.y, size.z),    //  |/	 |/
				glm::vec3(size.x, size.y, size.z),     //  3-----2
			};

			for (glm::vec3 &point : points) { point = matrix * glm::vec4(point, 1.0f); }

			constexpr std::array<unsigned int, 16> lines{
				0, 1, 2, 3, 0, 4, 7, 3,
				6, 7, 4, 5, 6, 2, 1, 5
			};

			return std::make_pair(points, lines);
		}

		
	}
}