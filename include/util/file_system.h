#ifndef GORE_FILE_SYSTEM_H
#define GORE_FILE_SYSTEM_H

#include "util/platform.h"

#include <string>
#include <string_view>

namespace gore
{
	class file_system
	{
	public:
		static std::string get_shader_path(std::string_view path);
		static std::string get_asset_path(std::string_view path);
	};

	inline std::string file_system::get_shader_path(std::string_view path)
	{
		return (util::platform::exe_path_ / "data" / "shaders" / path).string();
	}
	inline std::string file_system::get_asset_path(std::string_view path)
	{
		return (util::platform::exe_path_ / "data" / "assets" / path).string();
	}
}

#endif