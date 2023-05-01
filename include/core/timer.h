#ifndef GORE_TIMER_H
#define GORE_TIMER_H

#include <chrono>

namespace gore
{
    class timer
    {
    public:
        timer();
        static void time_scale(double scale) noexcept;
        double delta() const noexcept;
        void frame();
    private:
        double delta_;
        static double speed_;
        std::chrono::time_point<std::chrono::steady_clock> last_;
        std::chrono::time_point<std::chrono::steady_clock> current_;
    };

    inline double timer::delta() const noexcept
    {
        return delta_ * speed_;
    }
    inline void timer::frame()
    {
        current_ = std::chrono::steady_clock::now();
        delta_ = std::chrono::duration_cast<std::chrono::duration<double>>(current_ - last_).count();
        last_ = current_;
    }
}

#endif