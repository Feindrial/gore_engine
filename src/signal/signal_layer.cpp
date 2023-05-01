#include "signal/signal_layer.h"

namespace gore
{
	signal_layer *signal_layer::singleton_ = nullptr;

	signal_layer::signal_layer()
	{
		singleton_ = this;
	}	
	// const std::vector<signal_layer::signal_handler>& signal_layer::operator[](size_t n) const noexcept
	// {
	// 	return handlers_[n];
	// }
	void signal_layer::invoke(signal *sgnl)
	{
		for (const signal_handler &handle : handlers_[sgnl->sc_])
		{
			handle(sgnl);
			if (sgnl->handled_)
				break;
		}
		delete sgnl;
	}
}