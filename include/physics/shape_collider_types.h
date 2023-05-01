#ifndef GORE_SHAPE_COLLIDER_TYPES_H
#define GORE_SHAPE_COLLIDER_TYPES_H

#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

#include "scene/transform.h"
#include "scene/spatial.h"

namespace gore
{
    namespace physics
    {
        struct shape_collider : public spatial
        {
            shape_collider() = default;
            shape_collider(const transform &trans) : spatial{ trans } {}
        };

        struct box_collider : public shape_collider
        {
            explicit box_collider(const glm::vec3 &size) : size_{ size } {}
            box_collider(const glm::vec3 &size, const transform &trans) : shape_collider{ trans }, size_{ size } {}

            glm::vec3 size_;
        };
    }
}

#endif