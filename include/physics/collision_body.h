#ifndef GORE_COLLISION_BODY_H
#define GORE_COLLISION_BODY_H

#include "physics/shape_collider_types.h"
#include "scene/model_instance.h"
#include "scene/spatial.h"

#include <vector>
#include <string_view>

namespace gore
{
    namespace physics
    {
        class collision_body : public spatial
        {
        public:
            collision_body() = default;
            void add_shape(glm::vec3 size);
            box_collider& get_shape(size_t index);
            void delete_shape(size_t index);
            std::vector<box_collider>& get_shapes();
        private:
            std::vector<box_collider> shapes_;
        };

        inline void collision_body::add_shape(glm::vec3 size)
        { 
            shapes_.emplace_back(size);
            add_child(&shapes_.back());
        }
        inline box_collider& collision_body::get_shape(size_t index) 
        {
            return shapes_[index];            
        }
        inline void collision_body::delete_shape(size_t index)
        {
            shapes_.erase(shapes_.begin() + index);
        }
        inline std::vector<box_collider>& collision_body::get_shapes()
        {
            return shapes_;
        }
        
    }
}

#endif