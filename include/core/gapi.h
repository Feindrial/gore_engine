#ifndef GORE_GAPI_H
#define GORE_GAPI_H

#include "core/window.h"

#include <string_view>

namespace gore
{
    class gapi
    {
    public:
        window* window_api(std::string_view title, bool fullscreen = false);
        window* window_api(std::string_view title, int width, int height, bool fullscreen = false);
    private:
        window *window_api_;
    };
}

#endif