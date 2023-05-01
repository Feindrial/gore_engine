#ifndef GORE_SHADER_STORAGE_BUFFER_H
#define GORE_SHADER_STORAGE_BUFFER_H

#include "glm/mat4x4.hpp"
#include "glad/glad.h"

#include "util/util.h"
#include "gl/material.h"
#include "gl/dynamic_buffer.h"

#include <array>
#include <cstring>

namespace gore
{
    namespace gl
    {
        class shader_storage_buffer : public dynamic_buffer
        {
        public:
            void bind() const noexcept;
            void unbind() const noexcept;
            unsigned int push(const independent_uniform_data *data, unsigned int length = sizeof(independent_uniform_data));
            unsigned int push(const independent_uniform_data &data, unsigned int length = sizeof(independent_uniform_data));
        };

        inline void shader_storage_buffer::bind() const noexcept
        {
            glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 3, id_, section_ * size_, size_);
        }
        inline void shader_storage_buffer::unbind() const noexcept
        {

        }
        inline unsigned int shader_storage_buffer::push(const independent_uniform_data *data, unsigned int length)
		{
            unsigned int temp = current_;
			std::memcpy((char*)map_ + section_ * size_ + current_, data, length);
			current_ += length;
            return temp;
		}
        inline unsigned int shader_storage_buffer::push(const independent_uniform_data &data, unsigned int length)
		{
            unsigned int temp = current_;
			std::memcpy((char*)map_ + section_ * size_ + current_, &data, length);
			current_ += length;
            return temp;
		}
    }
}

#endif