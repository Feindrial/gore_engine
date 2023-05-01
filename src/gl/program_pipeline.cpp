#include "gl/program_pipeline.h"

#include "glad/glad.h"

namespace gore
{
    namespace gl
    {
        program_pipeline::program_pipeline()
        {
            glGenProgramPipelines(1, &id_);
        }
    }
}