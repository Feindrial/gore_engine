#include "gl/uniconstant_buffer.h"

#include "glad/glad.h"

#include "util/util.h"

namespace gore
{
    namespace gl
    {
        uniconstant_buffer::uniconstant_buffer(unsigned int size, util::information_size info) :
            buffer{ size * info }
        {
            glNamedBufferStorage(id_, size_, nullptr, GL_DYNAMIC_STORAGE_BIT);
        }
    }
}