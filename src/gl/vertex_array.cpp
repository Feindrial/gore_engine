#include "gl/vertex_array.h"

#include "glad/glad.h"
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"

#include <vector>
#include <variant>

namespace gore
{
	namespace gl
	{
		vertex_array::vertex_array(const mesh::vertex_layout &attributes, bool draw_counter) :
			stride_(formats.back().binding_index_ + 1 , 0)
		{
			glCreateVertexArrays(1, &id_);

			for (size_t i = 0; i < formats.size() - 1; i++) //last element is counter or else
			{
				glEnableVertexArrayAttrib(id_, i);
				glVertexArrayAttribBinding(id_, i, formats[i].binding_index_);
				glVertexArrayAttribFormat(id_, i,
											   formats[i].size_,
											   formats[i].type_,
											   GL_FALSE,
											   stride_[formats[i].binding_index_]);
				stride_[formats[i].binding_index_] += formats[i].size_ * formats[i].byte_;
			}

			glEnableVertexArrayAttrib(id_, formats.size() - 1);
			glVertexArrayAttribBinding(id_, formats.size() - 1, formats.back().binding_index_);
			// if last element is counter
			if (draw_counter)
			{
				glVertexArrayAttribIFormat(id_,
										   formats.size() - 1, 
										   formats.back().size_, 
										   formats.back().type_, 
										   stride_[formats.back().binding_index_]);
				stride_[formats.back().binding_index_] += formats.back().size_ * formats.back().byte_;
				glVertexArrayBindingDivisor(id_, formats.back().binding_index_, 1);
			}
			else 
			{
				glVertexArrayAttribFormat(id_,
										   formats.size() - 1, 
										   formats.back().size_, 
										   formats.back().type_, 
										   GL_FALSE,
										   stride_[formats.back().binding_index_]);
				stride_[formats.back().binding_index_] += formats.back().size_ * formats.back().byte_;
			}
			

			/*glEnableVertexArrayAttrib(id_, 0);
			glVertexArrayAttribBinding(id_, 0, 0);
			glVertexArrayAttribFormat(id_, 0, 3, GL_FLOAT, GL_FALSE, stride_);
			stride_ += sizeof(glm::vec3);
			glEnableVertexArrayAttrib(id_, 1);
			glVertexArrayAttribBinding(id_, 1, 0);
			glVertexArrayAttribFormat(id_, 1, 3, GL_FLOAT, GL_FALSE, stride_);
			stride_ += sizeof(glm::vec3);
			glEnableVertexArrayAttrib(id_, 2);
			glVertexArrayAttribBinding(id_, 2, 0);
			glVertexArrayAttribFormat(id_, 2, 3, GL_FLOAT, GL_FALSE, stride_);
			stride_ += sizeof(glm::vec3);
			glEnableVertexArrayAttrib(id_, 3);
			glVertexArrayAttribBinding(id_, 3, 0);
			glVertexArrayAttribFormat(id_, 3, 3, GL_FLOAT, GL_FALSE, stride_);
			stride_ += sizeof(glm::vec3);
			glEnableVertexArrayAttrib(id_, 4);
			glVertexArrayAttribBinding(id_, 4, 0);
			glVertexArrayAttribFormat(id_, 4, 2, GL_FLOAT, GL_FALSE, stride_);
			stride_ += sizeof(glm::vec2);*/
		}
		vertex_array::~vertex_array()
		{
			glDeleteVertexArrays(1, &id_);
		}
		const vertex_array& vertex_array::bind_buffer(unsigned int index, unsigned int vertex) const
		{
			for (size_t i = 0; i < stride_.size(); i++)
			{
				glVertexArrayVertexBuffer(id_, i, vertex, 0, stride_[i]);
			}
			glVertexArrayElementBuffer(id_, index);
			return *this;
		}
		const vertex_array& vertex_array::bind_buffer(unsigned int index, unsigned int vertex, unsigned int draw) const
		{
			for (size_t i = 0; i < stride_.size(); i++)
			{
				glVertexArrayVertexBuffer(id_, i, vertex, 0, stride_[i]);
			}
			glVertexArrayVertexBuffer(id_, stride_.size() - 1, draw, 0, stride_.back()); //draw counter
			glVertexArrayElementBuffer(id_, index);
			return *this;
		}
	}
}