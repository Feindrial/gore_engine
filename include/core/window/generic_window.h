#pragma once

#include "core/gorecore.h"

namespace gore
{
	enum class window_mode
	{
		fullscreen,
		windowed_fullscreen,
		windowed
	};

	class generic_window
	{
	protected:
		virtual void resize_window(i32 x, i32 y, i32 width, i32 height) = 0;
		virtual void move_window(i32 x, i32 y) = 0;
		virtual void set_window_mode(window_mode mode) = 0;
		virtual void get_native_handle() = 0;
		virtual void maximize() = 0;
		virtual void iconify() = 0;
		virtual void restore() = 0;
		virtual void show() = 0;
		virtual void hide() = 0;
	};
}