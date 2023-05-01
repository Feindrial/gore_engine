#ifndef GORE_PROGRAM_PIPELINE_H
#define GORE_PROGRAM_PIPELINE_H

#include "gl/globject.h"
#include "gl/program.h"
#include "glad/glad.h"
#include "util/platform.h"

#include <vector>

namespace gore
{
    namespace gl
    {
        class program_pipeline : public globject
        {
        public:
            program_pipeline();
            void attach(const program *prg);
        private:
            std::vector<program*> programs_;
        };

        __FORCE_INLINE void program_pipeline::attach(const program *prg)
        {
            //error programs contains same type of shaders
            
            glUseProgramStages(id_, prg->get_program_type(), prg->id());
        }

    }
}

#endif