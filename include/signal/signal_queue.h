#ifndef GORE_SIGNAL_QUEUE
#define GORE_SIGNAL_QUEUE

#include "signal/signal_types.h"

#include <vector>
#include <array>

namespace gore
{
	class signal_layer;

	class signal_queue
	{
	public:	
		signal_queue();
		static signal_queue* get() { return singleton_; }
		void add_signal(signal *sgnl);
		void handle(non_time_based_signal ctgry);
		void handle(time_based_signal ctgry, double delta);
		void handle_all_time_based(double delta);
		void handle_all_non_time_based();
		void handle_all(double delta);
	private:
		void clear();
		static signal_queue *singleton_;
		std::array<std::vector<signal*>, static_cast<size_t>(time_based_signal::capacity)> signals_; 
	};

	inline void signal_queue::add_signal(signal *sgnl)
	{
		signals_[sgnl->sc_].push_back(sgnl);
	}	
}

#endif