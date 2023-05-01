#ifndef GORE_VERTEX_ARRAY_H
#define GORE_VERTEX_ARRAY_H	

#include "glad/glad.h"

#include "core/platform_macros.h"
#include "gl/globject.h"

#include <vector>
#include <variant>

namespace gore
{
	namespace gl
	{	
		class vertex_array : public globject
		{
			struct vertex_format
			{
				unsigned int binding_index_;
				unsigned int size_;
				unsigned int type_;
				unsigned int byte_;
			};
		public:
			vertex_array();
			explicit vertex_array(bool draw_counter);
			~vertex_array();	
			friend bool operator==(const vertex_array &format1, const vertex_array &format2);
			friend bool operator!=(const vertex_array &format1, const vertex_array &format2);
			void set_index_type(unsigned int type);
			void push_back(unsigned int attribute_index,
						   unsigned int binding_index,
						   unsigned int dimension,
						   unsigned int type,
						   unsigned int size,
						   bool normalized = false);
			void enable(unsigned int index) const;
			void disable(unsigned int index) const;
			void bind() const;
			const vertex_array& bind_buffer(unsigned int index, unsigned int vertex) const;
			const vertex_array& bind_buffer(unsigned int index, unsigned int vertex, unsigned int draw) const;
		private:
			std::vector<unsigned int> layout_strides_;
			unsigned int index_type_;
		};

		__FORCE_INLINE void vertex_array::set_index_type(unsigned int type)
		{
			index_type_ = type;
		}
		__FORCE_INLINE void vertex_array::enable(unsigned int index) const
		{
			glEnableVertexArrayAttrib(id_, index);
		}
		__FORCE_INLINE void vertex_array::disable(unsigned int index) const
		{
			glDisableVertexArrayAttrib(id_, index);
		}
		__FORCE_INLINE void vertex_array::push_back(unsigned int attribute_index,
									 				unsigned int binding_index,
									 				unsigned int dimension,
									 				unsigned int type,
									 				unsigned int size,
									 				bool normalized)
		{
			int norm = normalized ? GL_TRUE : GL_FALSE;

			enable(attribute_index);
			glVertexArrayAttribBinding(id_, attribute_index, binding_index);
			glVertexArrayAttribFormat(id_, attribute_index, dimension, type, norm, layout_strides_[binding_index]);

			layout_strides_[binding_index] += dimension * size;
		}


		inline bool operator==(const vertex_array &format1, const vertex_array &format2)
		{
			return format1.id_ == format2.id_;
		}
		inline bool operator!=(const vertex_array &format1, const vertex_array &format2)
		{
			return !(operator==(format1, format2));
		}
		inline void vertex_array::bind() const
		{
			glBindVertexArray(id_);
		}		
	}
}

#endif