#ifndef GORE_PROGRAM_STORAGE_H
#define GORE_PROGRAM_STORAGE_H

#include "gl/shader.h"
#include "gl/program.h"
#include "util/platform.h"

#include <cstddef>
#include <unordered_map>
#include <vector>
#include <string>
#include <string_view>

namespace gore
{
    using pid = short;
    
    class program_storage
    {
    public:
        program_storage();
        ~program_storage();
        static program_storage* get() { return singleton_; }

        pid get_program_id();
        void attach_shader(pid id, std::string_view path, gl::shader_type type);
        void finalize(pid id);
    private:
        std::vector<gl::program> programs_;
        std::unordered_map<std::string, gl::shader> shaders_;
        
        static program_storage *singleton_;
    };

    __FORCE_INLINE void program_storage::finalize(pid id)
    {
        programs_[id].finalize();
    }
}

#endif