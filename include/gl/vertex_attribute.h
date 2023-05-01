#ifndef GORE_VERTEX_ATTRIBUTE_H
#define GORE_VERTEX_ATTRIBUTE_H

namespace gore
{
    namespace gl
    {
        enum class vertex_format : char
	    {
	    	f16,
	    	f32,
	    	i8,
	    	i16,
	    	i32,
	    	ui8,
	    	ui16,
	    	ui32
	    };

        struct vertex_attribute
        {
            vertex_format format_;
            unsigned char dimension_;
        };
    }
}

#endif