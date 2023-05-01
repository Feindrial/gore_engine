#include "core/timer.h"

namespace gore
{
    double timer::speed_ = 1.0;

    timer::timer() : delta_{ 0.0 }
    {

    }
    void timer::time_scale(double scale) noexcept
    {
        speed_ = scale;
    }
}