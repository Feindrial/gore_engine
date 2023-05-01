#ifndef GORE_FRAMEBUFFER_H
#define GORE_FRAMEBUFFER_H

namespace gore
{
	namespace gl
	{
		class framebuffer
		{
		public:
			framebuffer();
			~framebuffer();
			void bind() const;
			void unbind() const;
		};

		inline void framebuffer::bind() const
		{

		}
		inline void framebuffer::unbind() const
		{

		}
	}
}

#endif