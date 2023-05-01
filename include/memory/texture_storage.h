#ifndef GORE_TEXTURE_STORAGE_H
#define GORE_TEXTURE_STORAGE_H

#include "stb_image/stb_image.h"

#include "gl/material.h"
#include "gl/texture_pool.h"
#include "util/platform.h"
#include "memory/buffer_storage.h"
#include "util/file_system.h"

#include <vector>
#include <unordered_map>
#include <string>
#include <string_view>

namespace gore
{
    class buffer_storage;

    class texture_storage
    {
    public:
        texture_storage() { singleton_ = this; }
        gl::material get_material(std::string_view path);
        void reserve_material_data(int id);
        void push_material_data(int id, std::string_view path, gl::material_type type);
        static texture_storage* get() { return singleton_; }
    private:
        std::unordered_map<std::string, gl::material> materials_;
        std::unordered_map<int, gl::material_data> material_data_;
        static texture_storage *singleton_;
    };

    __FORCE_INLINE void texture_storage::reserve_material_data(int id)
    {
        material_data_.try_emplace(id);
    }
    __FORCE_INLINE void texture_storage::push_material_data(int id, std::string_view path, gl::material_type type)
    {
        std::string path_ = file_system::get_asset_path(path);

        if (!material_data_.contains(id))     
            reserve_material_data(id);

        int width, height, channels;
		unsigned char *data = stbi_load(path_.c_str(), &width, &height, &channels, 0);
        if (data)
        {
            gl::material mat = buffer_storage::get()->push_material_data(id, data, width, height, channels);

            if (!materials_.contains(path_))
                materials_.emplace(path_, mat);

            if (type == gl::material_type::diffuse)
                material_data_.find(id)->second.diffuse_ = mat;
            else if (type == gl::material_type::normal)
                material_data_.find(id)->second.normals_ = mat;
            else if (type == gl::material_type::specular)
                material_data_.find(id)->second.specular_ = mat;
            else if (type == gl::material_type::roughness)
                material_data_.find(id)->second.roughness_ = mat;
            else if (type == gl::material_type::ao)
                material_data_.find(id)->second.ao_ = mat;
            else
            {
                //not supported
            }
        }
        else
        {
            //texture load error
        }
    }
}

#endif