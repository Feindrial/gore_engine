#ifndef GORE_INPUT_H
#define GORE_INPUT_H

#include "core/interrupts.h"
#include "signal/signal_types.h"
#include "signal/signal_layer.h"

#include <unordered_set>

namespace gore
{
    class input 
    {
    public:
        input();
        static bool is_key_in_state(interrupt intrp);
        static bool is_mouse_button_in_state(interrupt intrp);
        static bool is_key_pushed(interrupt intrp);
        static void push_key(interrupt intrp);
        void update();
    private:
        static std::unordered_set<interrupt> pushed_keys_;
    };

    inline void input::update()
    {
        pushed_keys_.clear();
    }
}

#endif