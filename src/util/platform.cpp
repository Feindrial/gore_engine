#include "util/platform.h"

#include "glad/glad.h"

#include "core/platform_macros.h"

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <string>

namespace gore 
{
    namespace util 
    {
        namespace constants
        {
#ifdef __GORE_WINDOWS

            static std::string initialize_executable_path() 
            {
                std::string exe_path;
                char path[MAX_PATH];
                HMODULE handle = GetModuleHandle(NULL);
                if (handle != NULL) 
                {
                  GetModuleFileName(handle, path, (sizeof(path)));
                  exe_path = path;
                  auto last = std::unique(exe_path.begin(), exe_path.end());
                  exe_path.erase(last, exe_path.end());
                  // std::replace(exe_path.begin(), exe_path.end(), '\\', '/');
                  exe_path = exe_path.substr(0, exe_path.find_last_of('\\'));
                  return exe_path;
                }             

                return "";
            }
#endif    

#ifdef __GORE_LINUX

            static std::string initialize_executable_path() 
            {
                char result[PATH_MAX];
                ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
                const char *path;
                if (count != -1)
                {
                  path = dirname(result);
                  return std::string(path);              
                }

                return "";
            }

#endif  

            const std::filesystem::path executable_path_ = initialize_executable_path();

            void initialize_platform_constants()
            {
                glGetIntegerv(GL_MAX_FRAMEBUFFER_WIDTH, const_cast<int*>(&max_fbo_width_));
                glGetIntegerv(GL_MAX_FRAMEBUFFER_HEIGHT, const_cast<int*>(&max_fbo_height_));
                glGetIntegerv(GL_MAX_FRAMEBUFFER_LAYERS, const_cast<int*>(&max_fbo_layers_));
                glGetIntegerv(GL_MAX_FRAMEBUFFER_SAMPLES, const_cast<int*>(&max_fbo_samples_));
                glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, const_cast<int*>(&max_rbo_size_));
                glGetIntegerv(GL_MAX_TEXTURE_SIZE, const_cast<int*>(&max_tex_size_));
                glGetIntegerv(GL_MAX_TEXTURE_BUFFER_SIZE, const_cast<int*>(&max_tex_buffer_size_));
                glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, const_cast<int*>(&max_frag_tex_unit_));
                glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, const_cast<int*>(&max_combined_tex_unit_));
                glGetIntegerv(GL_MAX_UNIFORM_LOCATIONS, const_cast<int*>(&max_uniform_locations_));
                glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, const_cast<int*>(&max_ubo_size_));
                glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, const_cast<int*>(&max_ubo_bindings_));
                glGetIntegerv(GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS, const_cast<int*>(&max_ssbo_bindings_));
                glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, const_cast<int*>(&max_vertex_attributes_));
                glGetIntegerv(GL_MAX_VERTEX_ATTRIB_BINDINGS, const_cast<int*>(&max_vertex_attribute_bindings_));
            }
        }
    } 
} 