#ifndef GORE_PLATFORM_H
#define GORE_PLATFORM_H

#include <string>
#include <filesystem>

namespace gore
{
	namespace util
	{
		namespace constants
		{
			extern const std::filesystem::path executable_path_;

			extern const int max_fbo_width_;
			extern const int max_fbo_height_;
			extern const int max_fbo_layers_;
			extern const int max_fbo_samples_;
			extern const int max_rbo_size_;
			extern const int max_tex_size_;
			extern const int max_tex_buffer_size_;
			extern const int max_frag_tex_unit_;
			extern const int max_combined_tex_unit_;
			extern const int max_uniform_locations_;
			extern const int max_ubo_size_;
			extern const int max_ubo_bindings_;
			extern const int max_ssbo_bindings_;
			extern const int max_vertex_attributes_;
			extern const int max_vertex_attribute_bindings_;

			static std::string initialize_executable_path();
			void initialize_platform_constants();
		}		
	}
}

#endif