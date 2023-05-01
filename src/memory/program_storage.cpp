#include "memory/program_storage.h"

#include "util/file_system.h"

#include <utility>
#include <fstream>
#include <sstream>
#include <string>

namespace gore
{
    program_storage *program_storage::singleton_ = nullptr;

    program_storage::program_storage()
    {
        singleton_ = this;


        
    }
    program_storage::~program_storage()
    {

    }
    
    void program_storage::attach_shader(int id, std::string_view path, gl::shader_type type)
    {
        const std::string &path_ = file_system::get_shader_path(path);
        auto shdr = shaders_.find(path_);

        if (shdr != shaders_.end())
        {
            programs_[id].attach(&shdr->second);
        }
        else
        {
            //create the shader

            std::ifstream stream(path.data());
			std::stringstream sstream;
			sstream << stream.rdbuf();
			std::string code = sstream.str();
			


            auto it = shaders_.try_emplace(path_, code, type);
            programs_[id].attach(&it.first->second);
        }
    }
}