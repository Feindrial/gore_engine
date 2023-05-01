#include "gl/uniheap_buffer.h"

#include "glad/glad.h"

#include "util/util.h"

namespace gore
{
    namespace gl
    {
        uniheap_buffer::uniheap_buffer() :
            buffer{ 1 * util::information_size::kb }
        {
            glNamedBufferStorage(id_, size_ * poly_, nullptr, GL_DYNAMIC_STORAGE_BIT);
        }
    }
}