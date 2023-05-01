#include "gl/texture_pool.h"

#include "glad/glad.h"

#include "gl/material.h"
#include "util/util.h"

namespace gore
{
	namespace gl
	{
		texture_pool::texture_pool(gl::texture_format format, int width, int height) :
			width_{width}, height_{height}, format_{format}
		{
			glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &id_);
			glTextureParameteri(id_, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTextureParameteri(id_, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTextureParameteri(id_, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(id_, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTextureStorage3D(id_, 1, static_cast<GLenum>(format), width_, height_, depth_);

			texture_unit_ = util::lsb_first_zero(tex_unit_bitmap_);
			tex_unit_bitmap_.set(texture_unit_);

			if (texture_unit_ == -1)
				//no available tex unit
			
			glBindTextureUnit(texture_unit_, id_);
		}
		texture_pool::~texture_pool()
		{
			
		}
	}
}