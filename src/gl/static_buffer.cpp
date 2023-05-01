#include "gl/static_buffer.h"

#include "glad/glad.h"

namespace gore
{
	namespace gl
	{
		static_buffer::static_buffer()
		{		
			glNamedBufferStorage(id_, size_, nullptr, GL_DYNAMIC_STORAGE_BIT);
		}
		static_buffer::static_buffer(unsigned int size, util::information_size info) : buffer{ size * info }
		{
			glNamedBufferStorage(id_, size_, nullptr, GL_DYNAMIC_STORAGE_BIT);
		}
	}
}