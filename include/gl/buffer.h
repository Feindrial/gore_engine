#ifndef GORE_BUFFER_H
#define GORE_BUFFER_H

#include "gl/globject.h"
#include "util/platform.h"

namespace gore
{
    namespace gl
    {
        class buffer : public globject
        {
        public:
            buffer();
            explicit buffer(unsigned int size);
            ~buffer();

            unsigned int room() const;
        protected:
            const unsigned int size_;
            unsigned int current_;
        };

        __FORCE_INLINE unsigned int buffer::room() const
		{
			return size_ - current_;
		}
        
        
    }
}

#endif