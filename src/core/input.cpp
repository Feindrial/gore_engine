#include "GLFW/glfw3.h"

#include "core/input.h"
#include "core/window.h"
#include "signal/signal_layer.h"

#include <algorithm>
#include <utility>

namespace gore
{
    std::unordered_set<interrupt> input::pushed_keys_;

    input::input()
    {
        //signal_layer::get()->add_handler(std::bind(&input::key_pushed, this, std::placeholders::_1), non_time_based_signal::key_pushed);
    }
    bool input::is_key_in_state(interrupt intrp) 
    {
        int state = glfwGetKey(window::active().get(), intrp);
        return state == GLFW_PRESS || state == GLFW_REPEAT; 
    }
    bool input::is_mouse_button_in_state(interrupt intrp)
    {
        int state = glfwGetMouseButton(window::active().get(), intrp);
        return state == GLFW_PRESS || state == GLFW_REPEAT; 
    }
    bool input::is_key_pushed(interrupt intrp)
    {
        return pushed_keys_.contains(intrp);
    }
    void input::push_key(interrupt intrp)
    { 
        pushed_keys_.emplace(intrp); 
    }
}