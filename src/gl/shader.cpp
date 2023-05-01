#include "gl/shader.h"

#include "glad/glad.h"

#include "util/util.h"
#include "util/error_handler.h"

#include <string_view>

namespace gore
{
    namespace gl
    {
        shader::shader(std::string_view code, shader_type type) : 
            globject{ glCreateShader(static_cast<unsigned int>(type)) }, type_{ type }
        {
            glShaderSource(id_, 1, &util::unmove(code.data()), nullptr);
            glCompileShader(id_);

            int success = 0;
			glGetShaderiv(id_, GL_COMPILE_STATUS, &success);
			if (!success)
				error_handler::error(non_fatal::shader_compile);

            is_final_ = true;
        }
        shader::~shader()
        {

        }

        void shader::finalize()
        {
            if (!is_final_)
            {
                glCompileShader(id_);

                int success = 0;
			    glGetShaderiv(id_, GL_COMPILE_STATUS, &success);
			    if (!success)
			    	error_handler::error(non_fatal::shader_compile);
    
                is_final_ = true;
            }
        }
    }
}