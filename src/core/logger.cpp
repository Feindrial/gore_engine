#include "core/logger.h"

#include <vector>
#include <memory>
#include <utility>
#include <initializer_list>

namespace gore
{
	namespace error
	{
		logger::logger(std::initializer_list<std::shared_ptr<log_medium>> mediums) :
			err_intensity_{ error_type::fatal },
			mediums_{ std::move(mediums) }
		{
		}
		logger::logger(std::vector<std::shared_ptr<log_medium>> &mediums) :
			err_intensity_{ error_type::fatal },
			mediums_{ std::move(mediums) }
		{
		}
		void logger::change_log_level(error_type level)
		{
			err_intensity_ = level;
		}
	}
}