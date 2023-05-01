#include "gl/buffer.h"

#include "glad/glad.h"

#include "util/util.h"

namespace gore
{
    namespace gl
    {
        //default 4mb storage 
        buffer::buffer() : size_{ 4 * util::information_size::mb }, current_{ 0 }
        {
            glCreateBuffers(1, &id_);
        }
        buffer::buffer(unsigned int size) : size_{ size }, current_{ 0 }
        {
            glCreateBuffers(1, &id_);
        }
        buffer::~buffer()
        {
            glDeleteBuffers(1, &id_);
        }
    }
}