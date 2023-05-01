#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "core/window.h"
#include "core/error_macros.h"
#include "signal/signal_types.h"
#include "signal/signal_layer.h"
#include "signal/signal_queue.h"
#include "util/error_handler.h"

#include <utility>

namespace gore
{    
    window *window::active_ = nullptr;

    window::window() : should_close_{ false }
    {
        __GERR_NULLPTR_V(glfwInit(), util::error_type::fatal, "Window API cannot be initialized.");

        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
        glfwWindowHint(GLFW_MAXIMIZED, GLFW_FALSE);
        glfwWindowHint(GLFW_CENTER_CURSOR, GLFW_TRUE);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
#ifdef __GORE_DEBUG
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        width_ = mode->width;
        height_ = mode->height;
        window_ = glfwCreateWindow(width_, height_, "Project Gore", nullptr, nullptr);
        
        __GERR_NULLPTR_V(window_, util::error_type::fatal, "Window cannot be initialized.");

        glfwMakeContextCurrent(window_);
	    glfwFocusWindow(window_);
        glfwSetInputMode(window_, GLFW_STICKY_KEYS, GLFW_TRUE);
            
        __GERR_NULLPTR_V(gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)),
                         util::error_type::fatal,
                         "Graphic API functions cannot be loaded.");

        glViewport(0, 0, width_, height_);
        active_ = this;        
        //setting up window specific constant(never changed) signal callback functions    
        signal_layer::get()->add_handler(std::bind(&window::close_window, this, std::placeholders::_1), non_time_based_signal::window_close);
#ifdef __GORE_DEBUG
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(util::error_handler::opengl_debug, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
#endif
    }   
}