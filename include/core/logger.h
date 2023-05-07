#pragma once

#include "core/error_macros.h"
#include "core/log_medium.h"

#include <vector>
#include <chrono>
#include <memory>
#include <initializer_list>

namespace gore
{
	namespace error
	{
		class logger
		{
		public:
            logger(std::initializer_list<std::shared_ptr<log_medium>> mediums);
			logger(std::vector<std::shared_ptr<log_medium>> &mediums);

			void change_log_level(error_type level);
			template<typename... Args>
			void log(error_type level, Args... args);
		private:
			error_type err_intensity_;
			std::vector<std::shared_ptr<log_medium>> mediums_;
		};

		template<typename... Args>
		void logger::log(error_type level, Args... args)
		{
            if (level >= err_intensity_) {
                auto now = std::chrono::system_clock::now();
                auto now_c = std::chrono::system_clock::to_time_t(now);

                std::stringstream ss;
                ss << std::put_time(std::localtime(&now_c), "%Y-%m-%d %H:%M:%S") << " ";

                switch (level) {
                case error_type::fatal:
                    ss << "[FATAL] ";
                    break;
                case error_type::logic:
                    ss << "[LOGIC] ";
                    break;
                case error_type::shader:
                    ss << "[SHADER] ";
                    break;
                case error_type::warning:
                    ss << "[WARNING] ";
                    break;
                }

                ([&]
                {
                    ss << message << '\n';
                } (), ...);

                for (auto &medium : mediums_)
                {
                    medium->write(ss.str());
                }
            }
		}
	}
}