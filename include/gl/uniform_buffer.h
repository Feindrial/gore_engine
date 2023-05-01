#ifndef GORE_UNIFORM_BUFFER_H
#define GORE_UNIFORM_BUFFER_H

#include "gl/dynamic_buffer.h"

#include <string>

namespace gore
{
    namespace gl
    {
        class uniform_buffer : public dynamic_buffer<std::string>
        {
        public:
            uniform_buffer();
        };
    }
}

#endif