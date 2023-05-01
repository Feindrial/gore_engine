#ifndef GORE_MATERIAL_H
#define GORE_MATERIAL_H

#include "glad/glad.h"
#include "assimp/material.h"
#include "memory/texture_storage.h"

namespace gore
{    
    namespace gl
    {
        enum class material_type
        {
            diffuse   = aiTextureType_DIFFUSE,
            normal    = aiTextureType_NORMALS,
            specular  = aiTextureType_SPECULAR,
            roughness = aiTextureType_DIFFUSE_ROUGHNESS,
            ao        = aiTextureType_AMBIENT_OCCLUSION
        };

        enum class texture_format
        {
            gray        = GL_R8, 
            two_channel = GL_RG8,  
            rgb         = GL_RGB8, 
            rgba        = GL_RGBA8
        };

        struct texture
        {
            unsigned int layer_;
            int depth_;
        };

        struct material
        {
            pid id_;
        };

        struct material_data
        {
            material diffuse_;
            material normals_;
            material specular_;
            material roughness_;
            material ao_;
        };
    }
}

#endif