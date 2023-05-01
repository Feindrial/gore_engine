#ifndef GORE_ERROR_HANDLER_H
#define GORE_ERROR_HANDLER_H

#include "core/platform_macros.h"
#include "util/logger.h"

#include <string_view>
#include <type_traits>

namespace gore
{
    namespace util
    {
        enum class error_type : char
        {
            fatal,
            logic,
            warning,
            shader                      
        };

        struct file_info
        {
            const char *func_;
            const char *file_;
            int line_;
        };

        class error_handler
        {
        public:
            //Multi-use error handlers.
            template<typename... Args>
            static void unexpected(file_info f, error_type type, Args... args);
            template<typename T, typename... Args>
            static void null_pointer(file_info f, T pointer, error_type type, Args... args);
            
            //Single-use error handlers.
            template<typename... Args>
            static void warning(file_info f, std::string_view message, Args... args);
            template<typename T, typename U, typename... Args>
            static void enum_clash(file_info f, T container, U param1, U param2, Args... args);
            static void opengl_debug(unsigned int source,
                                     unsigned int type,
                                     unsigned int id,
                                     unsigned int severity,
                                     int length,
                                     const char *message,
                                     const void *user_param);
        protected:
        private:                
        };
        
        template<typename... Args>
        void error_handler::unexpected(file_info f, error_type type, Args... args)
        {
            logger::log_err_v(f.func_, f.file_, f.line_, "Something devious happened.", type, args...);
        }
        template<typename T, typename... Args>
        void error_handler::null_pointer(file_info f, T pointer, error_type type, Args... args)
        {                    
            if(!pointer) __UNLIKELY
                logger::log_err_v(f.func_, f.file_, f.line_, "Memory is not exist.", type, args...);
        }
        template<typename... Args>
        void error_handler::warning(file_info f, std::string_view message, Args... args)
        {
            logger::log_err_v(f.func_, f.file_, f.line_, message, error_type::warning, args...);
        }
        template<typename T, typename U, typename... Args>
        void error_handler::enum_clash(file_info f, T container, U param1, U param2, Args... args)
        {
            char which = 0;
            for (const auto &elem :container) 
            {
                if (elem == param1)
                {
                    which = 1;
                    break;
                }
                else if (elem == param2)
                {
                    which = 2;
                    break;
                }
            }

            if (which)
            {
                if (which == 1)
                {
                    for (const auto &elem : container)
                    {
                        if (elem == param2)
                        {
                            logger::log_err_v(f.func_, f.file_, f.line_, "Enum parameters clash with each other.", error_type::logic, args...);
                            break;
                        }
                    }
                }
                else if (which == 2)        
                {
                    for (const auto &elem : container)
                    {
                        if (elem == param1)
                        {
                            logger::log_err_v(f.func_, f.file_, f.line_, "Enum parameters clash with each other.", error_type::logic, args...);
                            break;
                        }
                    }
                }
            }

        }
    }
}

#endif