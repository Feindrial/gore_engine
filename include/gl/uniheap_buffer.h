#ifndef GORE_UNIHEAP_BUFFER_H
#define GORE_UNIHEAP_BUFFER_H

#include "gl/buffer.h"
#include "glad/glad.h"
#include "util/platform.h"

namespace gore
{
    namespace gl
    {
        class uniheap_buffer : public buffer
        {
        public:
            uniheap_buffer();

            void push(const void *data, unsigned int length);
        private:
            unsigned int poly_ = 1;
        };

        __FORCE_INLINE void uniheap_buffer::push(const void *data, unsigned int length)
        {
            if (room() < length)
            {
                poly_ *= 2;
                
                unsigned int new_id;
                glCreateBuffers(1, &new_id);
                glNamedBufferStorage(new_id, size_ * poly_, nullptr, GL_DYNAMIC_STORAGE_BIT);

                glCopyBufferSubData(id_, new_id, 0, 0, current_);
                glDeleteBuffers(1, &id_);
                id_ = new_id;
            }

            glNamedBufferSubData(id_, current_, length, data);
            current_ += length;
        }
    }
}

#endif