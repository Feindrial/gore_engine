#include "memory/texture_storage.h"

#include "util/file_system.h"

namespace gore
{
    texture_storage *texture_storage::singleton_ = nullptr;

    gl::material texture_storage::get_material(std::string_view path)
	{
        // std::string path_ = file_system::get_asset_path(path);
        // auto mtrl = materials_.find(path_);

        // if (mtrl != materials_.cend())
        // {
        //     return mtrl->second;
        // }
        // else
        // {
        //     int width, height, channels;
		//     unsigned char *data = stbi_load(path_.c_str(), &width, &height, &channels, 0);
        //     if (data)
        //     {
        //         unsigned int format;
        //         if (channels == 1)
        //             format = GL_R8;
        //         else if (channels == 2)
        //             format = GL_RG8;
        //         else if (channels == 3)
        //             format = GL_RGB8;
        //         else if (channels == 4)
        //             format = GL_RGBA8;

        //         for (size_t i = 0; i < tex_pools_.size(); i++)
        //         {
		//     		if (tex_pools_[i].is_applicable(format, width, height))
		//     		{
		//     			int depth = tex_pools_[i].create_texture(data);
        //                 return materials_.try_emplace(path_, path_, i, depth).first->second.tex_;
		//     		}
        //         }       //let the materials constructor handle this code         
        //         int depth = tex_pools_.emplace_back(format, channels, width, height).create_texture(data);
        //         return materials_.try_emplace(path_, path_, tex_pools_.size() - 1, depth).first->second.tex_;
        //     }
        //     else
        //     {
        //         //texture load error
        //     }
        // }
	}
}