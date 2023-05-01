#include "scene/spatial.h"

namespace gore
{
    spatial::spatial() : dirty_{ false }
    {

    }
    spatial::spatial(const transform &trans) : local_trans_{ trans }, dirty_{ false }
    {

    }

}