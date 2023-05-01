#ifndef GORE_OBB_H
#define GORE_OBB_H

#include "glm/vec3.hpp"

namespace gore
{
    struct obb
    {
    
        glm::vec3 c;
        glm::vec3 e;
        glm::vec3 u;
    };
}

#endif