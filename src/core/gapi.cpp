#include "core/gapi.h"

#include "core/window.h"

//#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "core/window.h"
#include "core/error_macros.h"
#include "signal/signal_types.h"
#include "signal/signal_layer.h"
#include "signal/signal_queue.h"
#include "util/error_handler.h"

#include <utility>

#include <string_view>

namespace gore
{
    window* gapi::window_api(std::string_view title, bool fullscreen)
    {
        if (window_api_)
        {
            __GERR_WARNING("Window API already initialized.",
                           "Multiple windows is not supported.",
                           "Window settings will be updated now."
                           "If you're trying get the window you can do it via gapi::get_window_api.");
            return window_api_;
        }

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
        int width = mode->width;
        int height = mode->height;

        GLFWwindow *glwfwindow;
        if (fullscreen)
            glwfwindow = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);
        else
            glwfwindow = glfwCreateWindow(width, height, title.data(), monitor, nullptr);

        __GERR_NULLPTR_V(glwfwindow, util::error_type::fatal, "Window cannot be initialized.");

        glfwMakeContextCurrent(glwfwindow);
	glfwFocusWindow(glwfwindow);
        glfwSetInputMode(glwfwindow, GLFW_STICKY_KEYS, GLFW_TRUE);
            
        __GERR_NULLPTR_V(gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)),
                         util::error_type::fatal,
                         "Graphic API functions cannot be loaded.");

        glViewport(0, 0, width, height);

        window_api_ = new window(width, height);
        
#ifdef __GORE_DEBUG
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(util::error_handler::opengl_debug, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
#endif

        return window_api_;
    }
    window* gapi::window_api(std::string_view title, int width, int height, bool fullscreen)
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
        
        GLFWwindow *glwfwindow;
        if (fullscreen)
            glwfwindow = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);
        else
            glwfwindow = glfwCreateWindow(width, height, title.data(), glfwGetPrimaryMonitor(), nullptr);
        
        __GERR_NULLPTR_V(glwfwindow, util::error_type::fatal, "Window cannot be initialized.");

        glfwMakeContextCurrent(glwfwindow);
	    glfwFocusWindow(glwfwindow);
        glfwSetInputMode(glwfwindow, GLFW_STICKY_KEYS, GLFW_TRUE);
            
        __GERR_NULLPTR_V(gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)),
                         util::error_type::fatal,
                         "Graphic API functions cannot be loaded.");

        glViewport(0, 0, width, height);
        
        window_api_ = new window(width, height);

#ifdef __GORE_DEBUG
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(util::error_handler::opengl_debug, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
#endif

        return window_api_;
    }
}