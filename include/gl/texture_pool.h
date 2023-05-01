#ifndef GORE_TEXTURE_POOL_H
#define GORE_TEXTURE_POOL_H

#include "glad/glad.h"

#include "gl/material.h"
#include "util/platform.h"
#include "util/util.h"

#include <array>
#include <map>
#include <bitset>

namespace gore
{
	namespace gl
	{
		class texture_pool
		{
		public:
			texture_pool(gl::texture_format format, int width, int height);
			~texture_pool();
			bool is_applicable(gl::texture_format format, int width, int height) const;
			int create_texture(const void *data);
			unsigned int unit() const;
			void bind();
			void unbind() const;
		private:
			static constexpr char max_texture_unit = 16;
			static constexpr char depth_ = 8;
			inline static std::map<gl::texture_format, GLenum> format_lookup_ = {
				{gl::texture_format::gray, GL_RED},
				{gl::texture_format::two_channel, GL_RG},
				{gl::texture_format::rgb, GL_RGB},
				{gl::texture_format::rgba, GL_RGBA}
			};
			inline static std::bitset<max_texture_unit> tex_unit_bitmap_;

			unsigned int texture_unit_;
			unsigned int id_;
			int width_;
			int height_;
			gl::texture_format format_;
			std::bitset<depth_> pool_bitmap_;
		};

		__FORCE_INLINE void texture_pool::bind()
		{
			texture_unit_ = util::lsb_first_zero(tex_unit_bitmap_);
			tex_unit_bitmap_.set(texture_unit_);
			
			glBindTextureUnit(texture_unit_, 0);
		}
		__FORCE_INLINE void texture_pool::unbind() const
		{
			tex_unit_bitmap_.set(texture_unit_, false);

			glBindTextureUnit(texture_unit_, id_);
		}
		__FORCE_INLINE unsigned int texture_pool::unit() const
		{
			return texture_unit_;
		}
		__FORCE_INLINE bool texture_pool::is_applicable(gl::texture_format format, int width, int height) const
		{
			return !pool_bitmap_.all() && width == width_ && height == height_ && format == format_;
		}
		__FORCE_INLINE int texture_pool::create_texture(const void *data)
		{
			int loc = util::lsb_first_zero(pool_bitmap_);
			pool_bitmap_.set(loc);

			glTextureSubImage3D(id_, 0, 0, 0, loc, width_, height_, 1,
							    format_lookup_[format_], GL_UNSIGNED_BYTE, data);
			return loc;
		}
	}
}

#endif