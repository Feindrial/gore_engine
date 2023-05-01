#include "gl/constant_buffer.h"

#include "glad/glad.h"

namespace gore
{
	namespace gl
	{
		constant_buffer::constant_buffer()
		{
			glNamedBufferStorage(id_, size_, nullptr, GL_DYNAMIC_STORAGE_BIT);
		}
		constant_buffer::constant_buffer(unsigned int size, util::information_size info) : 
			buffer{ size * info }
		{
			glNamedBufferStorage(id_, size_, nullptr, GL_DYNAMIC_STORAGE_BIT);
		}
	}
}