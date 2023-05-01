#ifndef GORE_LOGGER_H
#define GORE_LOGGER_H

#include <iostream>

namespace gore
{
    namespace util
    {
        enum class error_type : char;

        class logger
        {
        public:
            static void log_err(const char *function,
                                const char *file,
                                int line,
                                std::string_view error,
                                error_type type);
            template<typename... Args>
            static void log_err_v(const char *function,
                                  const char *file,
                                  int line,
                                  std::string_view error,
                                  error_type type,                                  
                                  Args... args);
        private:    
            static std::ostream &stream;    
        };

        template<typename... Args>
        void logger::log_err_v(const char *function,
                               const char *file,
                               int line,
                               std::string_view error,                             
                               error_type type,                               
                               Args... args)
        {
            log_err(function, file, line, error, type);
            
            if (sizeof...(args) > 0)
                ((stream << args << ' '), ...);
        }
    }
}

#endif 