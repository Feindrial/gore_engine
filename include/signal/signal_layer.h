#ifndef GORE_SIGNAL_LAYER_H
#define GORE_SIGNAL_LAYER_H

#include "signal/signal_types.h"
#include "signal/signal_queue.h"

#include <array>
#include <vector>
#include <functional>

namespace gore
{   
    class signal_layer
    {
    public:
        using signal_handler = std::function<void(signal*)>;
        signal_layer();
        static signal_layer* get() { return singleton_; }
        //const std::vector<signal_handler>& operator[](size_t n) const noexcept;
        void add_handler(const signal_handler &handler, non_time_based_signal ctgry);
        void add_handler(const signal_handler &handler, time_based_signal ctgry);
        void delete_handler(signal_handler handler);
        void invoke(signal *sgnl);
    private:
        static signal_layer *singleton_;
        std::array<std::vector<signal_handler>, static_cast<size_t>(time_based_signal::capacity)> handlers_;
    };

    inline void signal_layer::add_handler(const signal_handler &handler, non_time_based_signal ctgry)
    {        
        handlers_[static_cast<char>(ctgry)].push_back(handler);
    }
    inline void signal_layer::add_handler(const signal_handler &handler, time_based_signal ctgry)
    {        
        handlers_[static_cast<char>(ctgry)].push_back(handler);
    }
    inline void signal_layer::delete_handler(signal_handler handler)
    {

    }
}

#endif