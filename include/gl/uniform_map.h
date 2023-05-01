#ifndef GORE_UNIFORM_MAP_H
#define GORE_UNIFORM_MAP_H

#include "glad/glad.h"

#include <unordered_map>
#include <string>

//NOT SUPPORTED TYPES
//  ->image types
//  ->1d and 3d sampler types
//  ->atomic types
//  ->double precision

namespace gore
{
    namespace gl
    {
        inline std::unordered_map<unsigned int, const std::string> uniform_map{
            { GL_FLOAT,                                        "float" },
            { GL_FLOAT_VEC2,                                   "vec2" },
            { GL_FLOAT_VEC3,                                   "vec3" },
            { GL_FLOAT_VEC4,                                   "vec4" },
            { GL_INT,                                          "int" },
            { GL_INT_VEC2,                                     "ivec2" },
            { GL_INT_VEC3,                                     "ivec3" },
            { GL_INT_VEC4,                                     "ivec4" },
            { GL_UNSIGNED_INT,                                 "uint" },
            { GL_UNSIGNED_INT_VEC2,                            "uvec2" },
            { GL_UNSIGNED_INT_VEC3,                            "uvec3" },
            { GL_UNSIGNED_INT_VEC4,                            "uvec4" },
            { GL_FLOAT_MAT2,                                   "mat2" },
            { GL_FLOAT_MAT3,                                   "mat3" },
            { GL_FLOAT_MAT4,                                   "mat4" },
            { GL_FLOAT_MAT2x3,                                 "mat2x3" },
            { GL_FLOAT_MAT2x4,                                 "mat2x4" },
            { GL_FLOAT_MAT3x2,                                 "mat3x2" },
            { GL_FLOAT_MAT3x4,                                 "mat3x4" },
            { GL_FLOAT_MAT4x2,                                 "mat4x2" },
            { GL_FLOAT_MAT4x3,                                 "mat4x3" }
        };
    }
}




// GL_SAMPLER_1D                                   sampler1D    
// GL_SAMPLER_2D                                   sampler2D    
// GL_SAMPLER_3D                                   sampler3D    
// GL_SAMPLER_CUBE                                 samplerCube    
// GL_SAMPLER_1D_SHADOW                            sampler1DShadow    
// GL_SAMPLER_2D_SHADOW                            sampler2DShadow    
// GL_SAMPLER_1D_ARRAY                             sampler1DArray    
// GL_SAMPLER_2D_ARRAY                             sampler2DArray    
// GL_SAMPLER_1D_ARRAY_SHADOW                      sampler1DArrayShadow    
// GL_SAMPLER_2D_ARRAY_SHADOW                      sampler2DArrayShadow    
// GL_SAMPLER_2D_MULTISAMPLE                       sampler2DMS    
// GL_SAMPLER_2D_MULTISAMPLE_ARRAY                 sampler2DMSArray    
// GL_SAMPLER_CUBE_SHADOW                          samplerCubeShadow    
// GL_SAMPLER_BUFFER                               samplerBuffer    
// GL_SAMPLER_2D_RECT                              sampler2DRect    
// GL_SAMPLER_2D_RECT_SHADOW                       sampler2DRectShadow    
// GL_INT_SAMPLER_1D                               isampler1D    
// GL_INT_SAMPLER_2D                               isampler2D    
// GL_INT_SAMPLER_3D                               isampler3D    
// GL_INT_SAMPLER_CUBE                             isamplerCube    
// GL_INT_SAMPLER_1D_ARRAY                         isampler1DArray    
// GL_INT_SAMPLER_2D_ARRAY                         isampler2DArray    
// GL_INT_SAMPLER_2D_MULTISAMPLE                   isampler2DMS    
// GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY             isampler2DMSArray    
// GL_INT_SAMPLER_BUFFER                           isamplerBuffer    
// GL_INT_SAMPLER_2D_RECT                          isampler2DRect    
// GL_UNSIGNED_INT_SAMPLER_1D                      usampler1D    
// GL_UNSIGNED_INT_SAMPLER_2D                      usampler2D    
// GL_UNSIGNED_INT_SAMPLER_3D                      usampler3D    
// GL_UNSIGNED_INT_SAMPLER_CUBE                    usamplerCube    
// GL_UNSIGNED_INT_SAMPLER_1D_ARRAY                usampler2DArray    
// GL_UNSIGNED_INT_SAMPLER_2D_ARRAY                usampler2DArray    
// GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE          usampler2DMS    
// GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY    usampler2DMSArray    
// GL_UNSIGNED_INT_SAMPLER_BUFFER                  usamplerBuffer    
// GL_UNSIGNED_INT_SAMPLER_2D_RECT                 usampler2DRect    

#endif