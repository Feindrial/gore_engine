#ifndef GORE_SIGNAL_TYPES_H
#define GORE_SIGNAL_TYPES_H

#include "core/interrupts.h"

#include <utility>
#include <thread>

namespace gore
{
	enum class non_time_based_signal : char
	{
		window_close,
		window_resize,
		model_push,
		key_pushed,
		change_state,
		capacity
	};
	enum class time_based_signal : char
	{
		mouse_moved = static_cast<char>(non_time_based_signal::capacity),
		capacity
	};

	struct signal
	{
		signal(char sc) : sc_{ sc }, handled_{ false } {}
		virtual ~signal() {}
		char sc_;
		bool handled_;
		double delta_; //not every signal is time based so need to dispatch it actually but it will work for now
	};
	struct window_close_signal final : public signal
	{
		window_close_signal() : signal{ static_cast<char>(non_time_based_signal::window_close) } {}
	};
	struct window_resize_signal final : public signal
	{
		window_resize_signal(int width, int height) : signal{ static_cast<char>(non_time_based_signal::window_resize) }, width_{ width }, height_{ height } {}
		int width_, height_;
	};
	struct model_push_signal final : public signal
	{
		model_push_signal(std::thread &&thread) : signal{ static_cast<char>(non_time_based_signal::model_push) }, thread_{ std::move(thread) } {}
		std::thread thread_;
	};
	struct mouse_moved_signal final : public signal
	{
		mouse_moved_signal(float xpos, float ypos) : signal{ static_cast<char>(time_based_signal::mouse_moved) }, xpos_{ xpos }, ypos_{ ypos } {}
		float xpos_, ypos_;
	};
	struct key_pushed_signal final : public signal
	{
		key_pushed_signal(interrupt intrp) : signal{ static_cast<char>(non_time_based_signal::key_pushed) }, interrupt_{intrp} {}
		interrupt interrupt_;
	};
	struct change_state_signal final : public signal
	{
		change_state_signal() : signal{ static_cast<char>(non_time_based_signal::change_state) } {}
	};
}

#endif