#ifndef GORE_CONSTANT_BUFFER_H
#define GORE_CONSTANT_BUFFER_H

#include "glad/glad.h"

#include "gl/buffer.h"
#include "util/util.h"
#include "util/platform.h"


namespace gore
{
	namespace gl
	{
		class constant_buffer : public buffer
		{
		public:
			constant_buffer();
			constant_buffer(unsigned int size, util::information_size info);
			
			unsigned int push(void* data, unsigned int length);
		private:
		};


		__FORCE_INLINE unsigned int constant_buffer::push(void *data, unsigned int length)
		{
			int temp = current_;
			glNamedBufferSubData(id_, current_, length, data);
			current_ += length;
			return temp;
		}
	}
}

#endif