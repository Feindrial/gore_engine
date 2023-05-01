#ifndef GORE_UTIL_H
#define GORE_UTIL_H

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"

#include <type_traits>
#include <array>
#include <cmath>

namespace gore
{
	namespace util
	{
		enum information_size : unsigned int
		{
			b  = 1,
			kb = 1 << 10,
			mb = 1 << 20,
			gb = 1 << 30
		};
		enum class interpolation_type
		{
			linear
		};

		float interpolate(float min, float max, float value, interpolation_type type = interpolation_type::linear);
		std::pair<std::array<glm::vec3, 8>, std::array<unsigned int, 16>> get_cube(glm::vec3 center, glm::vec3 extend);
		std::pair<std::array<glm::vec3, 8>, std::array<unsigned int, 16>> get_cube(glm::vec3 size, glm::mat4 matrix);

		constexpr unsigned int bit(unsigned int bit)
		{
			return 1 << bit;
		}
		template<typename T>
		constexpr unsigned int lsb_first_zero(T t, unsigned int constraint = 0)
		{	

			if constexpr (std::is_class_v<T>) //bitset
			{
				constraint = t.size();

				for (size_t i = 0; i < constraint; i++)
    			{
    			    if ((t[0] & 1) == 0)
    			        return i;

    			    t >>= 1;
    			}
			}
            else
            {
                if (!constraint)
                    constraint = sizeof(t) * 8;

                for (size_t i = 0; i < constraint; i++)
                {
                    if ((t & 1) == 0)
                        return i;

                    t >>= 1;
                }
            }
				
    		return -1;
		}
		template<typename T>
		T& unmove(T&& t)
		{
			return t;
		}

	}
}

#endif