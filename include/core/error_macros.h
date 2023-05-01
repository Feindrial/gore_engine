#ifndef GORE_ERROR_MACROS_H
#define GORE_ERROR_MACROS_H

#include "util/error_handler.h"

// struct file_info
// {
//     const char *func_;
//     const char *file_;
//     int line_;
// };

//#define __GORE_FILE_INFO file_info{ __FUNCTION__, __FILE__, __LINE__ }


#define __GEVILSTR(str) #str
#define __GNOMONITOR
#define __GMONITOR(...) __VA_OPT__(,) __VA_ARGS__

#define __GERR(monitor_params, function, ...)               \
    function({ __FUNCTION__, __FILE__, __LINE__ }, __VA_ARGS__ monitor_params);
#define __GERR_EH(monitor_params, function, ...)                                         \
    gore::util::error_handler::function({ __FUNCTION__, __FILE__, __LINE__ }, __VA_ARGS__ monitor_params);

#define __GERR_UNEXPECTED(type, ...)                      \
    __GERR_EH(__GMONITOR(__VA_ARGS__), unexpected, type);

#define __GERR_ENUM_CLASH(container, param1, param2, ...)                                                                             \
    __GERR_EH(__GMONITOR(__GEVILSTR(param1), __GEVILSTR(param2) __VA_OPT__(,) __VA_ARGS__), enum_clash, container, param1, param2);

#define __GERR_NULLPTR(pointer, type)                     \
    __GERR_EH(__GNOMONITOR, null_pointer, pointer, type);

#define __GERR_NULLPTR_V(pointer, type, ...)                         \
    __GERR_EH(__GMONITOR(__VA_ARGS__), null_pointer, pointer, type);

#define __GERR_WARNING(message, ...)                      \
    __GERR_EH(__GMONITOR(__VA_ARGS__), warning, message);

#endif