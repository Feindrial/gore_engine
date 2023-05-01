#ifndef GORE_SHADER_H
#define GORE_SHADER_H

#include "glad/glad.h"

#include "gl/globject.h"
#include "util/platform.h"
#include "util/util.h"

#include <string_view>

namespace gore
{
    namespace gl
    {
        enum class shader_type
        {
            vertex       = GL_VERTEX_SHADER,
            tess_control = GL_TESS_CONTROL_SHADER,
            tess_eval    = GL_TESS_EVALUATION_SHADER,
            geometry     = GL_GEOMETRY_SHADER,
            fragment     = GL_FRAGMENT_SHADER,
            compute      = GL_COMPUTE_SHADER,
        };

        class shader : public globject
        {
        public:
            shader() = default;
            shader(std::string_view code, shader_type type);
            ~shader();
            
            shader_type get_shader_type() const;
            void attach_code(std::string_view code, shader_type type);
            void finalize();
        private:
            bool is_final_ = false;

            shader_type type_;
        };

        __FORCE_INLINE shader_type shader::get_shader_type() const
        {
            return type_;
        }
        __FORCE_INLINE void shader::attach_code(std::string_view code, shader_type type)
        {
            if (!is_final_)
            {
                id_ = glCreateShader(static_cast<unsigned int>(type));

                glShaderSource(id_, 1, &util::unmove(code.data()), nullptr);
            }
        }
        
    }
}

#endif