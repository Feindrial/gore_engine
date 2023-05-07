#pragma once

#include "core/log_medium.h"
#include "core/logger.h"

#include <memory>

namespace gore
{
	namespace error
	{
		extern std::shared_ptr<filelog_medium> basic_filelog;
		extern std::shared_ptr<consolelog_medium> basic_consolelog;

		extern logger f_logger;
		extern logger c_logger;
		extern logger fc_logger;
	}
}