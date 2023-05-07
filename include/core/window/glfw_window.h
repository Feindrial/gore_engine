#pragma once

#include "window/generic_window.h"
#include "core/gorecore.h"

namespace gore
{
	class glfw_window : public generic_window
	{
	public:
		void resize_window(i32 x, i32 y, i32 width, i32 height) override;
		void move_window(i32 x, i32 y) override;
		void set_window_mode(window_mode mode) override;
		void get_native_handle() override;
		void maximize() override;
		void iconify() override;
		void restore() override;
		void show() override;
		void hide() override;
	private:
		GLFWwindow *window_;
	};
}