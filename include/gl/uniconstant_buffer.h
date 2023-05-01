#ifndef GORE_UNICONSTANT_BUFFER_H
#define GORE_UNICONSTANT_BUFFER_H

#include "glad/glad.h"

#include "gl/buffer.h"
#include "util/util.h"
#include "util/platform.h"

namespace gore
{
    namespace gl
    {
        class uniconstant_buffer : public buffer
        {
        public:
            uniconstant_buffer(unsigned int size, util::information_size info);

            void push(const void *data, unsigned int length);
        };

        __FORCE_INLINE void uniconstant_buffer::push(const void *data, unsigned int length)
        {
            glNamedBufferSubData(id_, 0, length, data);
        }
    }
}

#endif