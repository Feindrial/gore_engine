#include "util/logger.h"
#include "util/error_handler.h"

#include <string>

namespace gore
{
    namespace util
    {
        std::ostream &logger::stream = std::cerr;

        void logger::log_err(const char *function,
                             const char *file,
                             int line,
                             std::string_view error,
                             error_type type)
        {
            std::string err_type;

            switch (type)
            {
            case error_type::fatal:
                err_type = "FATAL ERROR";
                break;
            case error_type::logic:
                err_type = "LOGIC ERROR";
                break;
            case error_type::warning:
                err_type = "WARNING";
                break;
            case error_type::shader:
                err_type = "SHADER ERROR";
                break;            
            default:
                break;
            }

            stream << err_type << " at " << file << ':' << function << " in line number " << line << ".\n";
            stream << error << '\n';
        }
    }
}