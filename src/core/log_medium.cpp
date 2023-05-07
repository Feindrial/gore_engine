#pragma once

#include "core/log_medium.h"

#include <string_view>
#include <iostream>

namespace gore
{
	namespace error
	{
		filelog_medium::filelog_medium(std::string_view filename) :
			out_file_{ filename.data(), std::ios_base::out }
		{			
		}
		void filelog_medium::write(const char *message)
		{
			out_file_ << message;
		}
		void filelog_medium::flush()
		{
			out_file_.flush();
		}

		void consolelog_medium::write(const char *message)
		{
			std::cout << message;
		}
		void consolelog_medium::flush()
		{
			std::cout.flush();
		}
	}
}