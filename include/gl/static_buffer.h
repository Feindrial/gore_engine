#ifndef GORE_STATIC_BUFFER_H
#define GORE_STATIC_BUFFER_H

#include "glad/glad.h"

#include "util/util.h"
#include "util/platform.h"
#include "gl/buffer.h"

#include <unordered_map>
#include <vector>

namespace gore
{
	namespace gl
	{
		class static_buffer : public buffer
		{
			struct memory_allocation_info
        	{
        	    unsigned int offset_;
        	    unsigned int current_offset;
        	    unsigned int size_;
        	};
		public:
			static_buffer();
			static_buffer(unsigned int size, util::information_size info);

			unsigned int reserve(int id, unsigned int length);
			void push(int id, const void *data, unsigned int length);
		private:
			std::unordered_map<int, memory_allocation_info> allocation_map_;
			std::vector<memory_allocation_info> free_blocks_;
		};

		__FORCE_INLINE unsigned int static_buffer::reserve(int id, unsigned int length)
		{
			for (size_t i = 0; i < free_blocks_.size(); i++)
			{
				if (free_blocks_[i].size_ >= length) //first fit algorithm
				{
					unsigned int offset = free_blocks_[i].offset_;
					allocation_map_.try_emplace(
						id,
						offset,
						offset,
						free_blocks_[i].size_
					);
					free_blocks_.erase(free_blocks_.cend() + i);
					return offset;
				}
			}
			
			if (room() > length)
			{
				unsigned int offset = current_;
				allocation_map_.try_emplace(
					id,
					offset,
					offset,
					length
				);
				current_ += length;
				return offset;
			}

			return -1;
		}
		__FORCE_INLINE void static_buffer::push(int id, const void *data, unsigned int length)
		{
			if (auto it = allocation_map_.find(id); it != allocation_map_.cend())
			{
				if (it->second.current_offset + length > it->second.offset_ + it->second.size_)
				{
					//not enough reserved memory
				}

				glNamedBufferSubData(id_, it->second.current_offset, length, data);
				it->second.current_offset += length;
			}
			else
			{
				//memory is not reserved
			}
		}
	}
}

#endif