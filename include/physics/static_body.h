#ifndef GORE_STATIC_BODY_H
#define GORE_STATIC_BODY_H

#include "physics/collision_body.h"
#include "scene/static_model.h"

#include <string_view>

namespace gore
{
    namespace physics
    {
        class static_body : public collision_body
        {
        public:
            static_body(std::string_view path);
        private:
        };
    }
}

#endif