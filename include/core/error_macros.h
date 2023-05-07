#pragma once

#include "core/basic_logger.h"

#include <string>
#include <string_view>

namespace gore
{
    namespace error
    {
        struct err_file_info
        {
            const char *func_;
            const char *file_;
            int line_;
        };
        enum class error_type : char
        {
            warning,
            logic,
            shader,
            fatal
        };

        template<typename... Args>
        struct log_err
        {
            log_err(err_file_info info, const char *error, error_type type, Args... args, const logger &lgr = fc_logger)
            {
                fc_logger.log(type, info, error, args);
            }
        };
        template <typename... Args>
        log_err(Args...) -> log_err<Args...>;
    }
}

#define __GORE_FILE_INFO gore::error::file_info{ __FUNCTION__, __FILE__, __LINE__ }
#define __GEVILSTR(str) #str
#define __GNOMONITOR
#define __GMONITOR(...) __VA_OPT__(,) __VA_ARGS__


#define __GERR(monitor_params, function, ...)               \
    function(__GORE_FILE_INFO, __VA_ARGS__ monitor_params);


#define __GERR_UNEXPECTED(type)                                           \
    __GERR(__GNOMONITOR, log_err_v, "Something devious happened.", type)) \

#define __GERR_UNEXPECTED_V(type, ...)                                               \
    __GERR(__GMONITOR(__VA_ARGS__), log_err_v, "Something devious happened.", type)) \

#define __GERR_WARNING(message, ...)                                                       \
    __GERR(__GMONITOR(__VA_ARGS__), log_err_v, message, gore::error::error_type::warning);

#define __GERR_NULLPTR(pointer, type)                              \
    if __UNLIKELY(!pointer)                                        \
        __GERR(__GNOMONITOR, log_err_v, "Pointer is null.", type);

#define __GERR_NULLPTR_V(pointer, type, ...)                                                      \
    if __UNLIKELY(!pointer)                                                                       \
        __GERR(__GMONITOR(__GEVILSTR(pointer) __VA_ARGS__), " pointer is null." log_err_v, type);