#include "GLFW/glfw3.h"

#include "signal/signal_queue.h"
#include "signal/signal_layer.h"
#include "signal/signal_types.h"
#include "core/window.h"

#include <algorithm>
#include "core/input.h"

namespace gore
{
	signal_queue *signal_queue::singleton_ = nullptr;

	signal_queue::signal_queue() 
	{
		singleton_ = this;

		GLFWwindow *wnd = window::active().get();
		glfwSetWindowUserPointer(wnd, reinterpret_cast<void*>(this));
		
        //setting up glfw callbacks    
		glfwSetWindowCloseCallback(wnd, [](GLFWwindow *window){
			signal_queue *queue{ reinterpret_cast<signal_queue*>(glfwGetWindowUserPointer(window)) };
			queue->add_signal(new window_close_signal);
		});
		glfwSetCursorPosCallback(wnd, [](GLFWwindow *window, double xpos, double ypos){
			signal_queue *queue{ reinterpret_cast<signal_queue*>(glfwGetWindowUserPointer(window)) };
			queue->add_signal(new mouse_moved_signal(xpos, ypos));
		});
		glfwSetKeyCallback(wnd, [](GLFWwindow *window, int key, int scancode, int action, int mods){
			if (action == GLFW_PRESS)
				input::push_key(key);
		});
	}
	void signal_queue::handle(non_time_based_signal ctgry)
	{
		for (signal *signal : signals_[static_cast<size_t>(ctgry)])
		{
			signal_layer::get()->invoke(signal);
		}
		signals_[static_cast<size_t>(ctgry)].resize(0);
	}
	void signal_queue::handle(time_based_signal ctgry, double delta)
	{
		for (signal *signal : signals_[static_cast<size_t>(ctgry)])
		{
			signal->delta_ = delta;
			signal_layer::get()->invoke(signal);
		}
		signals_[static_cast<size_t>(ctgry)].resize(0);
	}
	void signal_queue::handle_all_time_based(double delta)
	{
		for (size_t i = static_cast<size_t>(non_time_based_signal::capacity);
		 	 i < static_cast<size_t>(time_based_signal::capacity);
		 	 i++)
		{
			handle(static_cast<time_based_signal>(i), delta);
		}
	}
	void signal_queue::handle_all_non_time_based()
	{
		for (size_t i = 0;
		 	 i < static_cast<size_t>(non_time_based_signal::capacity);
		 	 i++)
		{
			handle(static_cast<non_time_based_signal>(i));
		}
	}
	void signal_queue::handle_all(double delta)
	{		
		handle_all_non_time_based();
		handle_all_time_based(delta);
	}

}