#ifndef GORE_WINDOW_H
#define GORE_WINDOW_H

#include "GLFW/glfw3.h"

#include "core/platform_macros.h"
#include "signal/signal_layer.h"
#include "signal/signal_types.h"

#include <string>
#include <vector>

namespace gore
{
    class window 
    {
    public:
        window();
        window(int width, int height);

        void change_title(std::string_view title) const;
        float width() const noexcept;
        float height() const noexcept;
        void update() const noexcept;
        void swap() const noexcept;
        bool should_close() const noexcept;
        [[nodiscard]] GLFWwindow* get() const noexcept;
        [[nodiscard]] static window& active() noexcept;
    protected:
    private:
        void close_window(signal *st);
        GLFWwindow *window_;
        static window *active_;
        int width_;
        int height_;  
        bool should_close_;
    };

    __FORCE_INLINE void window::change_title(std::string_view title) const
    {
        glfwSetWindowTitle(window_, title.data());
    }
    inline float window::width() const noexcept
    {
        return width_;
    }
    inline float window::height() const noexcept
    {
        return height_;
    }
    inline bool window::should_close() const noexcept 
    {
        return should_close_; 
    }
    [[nodiscard]] inline GLFWwindow* window::get() const noexcept 
    {
        return window_; 
    }
    [[nodiscard]] inline window& window::active() noexcept
    {
        return *active_;
    }
    inline void window::update() const noexcept
    {
        glfwPollEvents();
    } 
    inline void window::swap() const noexcept
    {
        glfwSwapBuffers(window_);
    }
    inline void window::close_window(signal *st) 
    {
        st->handled_ = true;
        should_close_ = true; 
    }
}

#endif