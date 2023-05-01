#ifndef GORE_BUFFER_STORAGE_H
#define GORE_BUFFER_STORAGE_H

#include "gl/static_buffer.h"
#include "gl/texture_pool.h"
#include "gl/uniform_buffer.h"
#include "gl/uniconstant_buffer.h"
#include "gl/uniheap_buffer.h"
#include "util/util.h"
#include "util/platform.h"
#include "memory/render_data_storage.h"

#include <unordered_set>
#include <map>
#include <unordered_map>
#include <vector>

namespace gore
{
    enum class buffer_hint
    {
        static_buffer  = util::bit(0),
        dynamic_buffer = util::bit(1)
    };

    class buffer_storage
    {
    public:
        buffer_storage();
        ~buffer_storage();
        static buffer_storage* get() { return singleton_; }
        void reserve_ubo_data(std::string name);
        void reserve_buffer_data(int id, int vertex_length_hint = 0, int index_length_hint = 0, buffer_hint hint = buffer_hint::static_buffer);
        void push_ubo_data(std::string name, const void *data, int length);
        void push_mesh_data(int id, const void *vertex_data, int vertex_length, const void *index_data, int index_length);
        void push_vertex_data(int id, const void *vertex_data, int vertex_length);
        void push_index_data(int id, const void *index_data, int index_length);
        gl::material push_material_data(int id, const void *data, int width, int height, int channels);
    private:
        std::vector<gl::static_buffer> static_buffers_;
        std::vector<gl::static_buffer> static_element_buffers_;
        std::vector<gl::texture_pool> tex_pool_;
        std::vector<gl::uniconstant_buffer> ubos_;
        std::vector<gl::uniconstant_buffer> unic_ssbos_;
        std::vector<gl::uniheap_buffer> unih_ssbos_;
        std::unordered_map<int, int> sb_map_;
        std::unordered_map<int, int> seb_map_;
        std::unordered_map<int, int> tex_map_;
        std::unordered_map<std::string, int> ubo_map_;
        
        static buffer_storage *singleton_;
    };

    __FORCE_INLINE void buffer_storage::push_ubo_data(std::string name, const void *data, int length)
    {
        if (auto it = ubo_map_.find(name); it == ubo_map_.end())
            reserve_ubo_data(name);
        else
        {
            
        }
            //ubos_[it->second].push(name, data, length);
    }
    __FORCE_INLINE void buffer_storage::push_mesh_data(int id, const void *vertex_data, int vertex_length, const void *index_data, int index_length)
    {
        push_vertex_data(id, vertex_data, vertex_length);
        push_index_data(id, index_data, index_length);
    }
    __FORCE_INLINE void buffer_storage::push_vertex_data(int id, const void *vertex_data, int vertex_length)
    {
        if (auto it = sb_map_.find(id); it == sb_map_.end())
            reserve_buffer_data(id);
        else
            static_buffers_[it->second].push(id, vertex_data, vertex_length);
    }
    __FORCE_INLINE void buffer_storage::push_index_data(int id, const void *index_data, int index_length)
    {
        if (auto it = sb_map_.find(id); it == sb_map_.end())
            reserve_buffer_data(id);
        else
            static_element_buffers_[it->second].push(id, index_data, index_length);
    }
    __FORCE_INLINE gl::material buffer_storage::push_material_data(int id, const void *data, int width, int height, int channels)
    {
        gl::texture_format format;
        if (channels == 1)
            format = gl::texture_format::gray;
        else if (channels == 2)
            format = gl::texture_format::two_channel;
        else if (channels == 3)
            format = gl::texture_format::rgb;
        else if (channels == 4)
            format = gl::texture_format::rgba;
        else
            //not supported
        
        for (size_t i = 0; i < tex_pool_.size(); i++)
        {
            if (tex_pool_[i].is_applicable(format, width, height))
            {
                int depth = tex_pool_[i].create_texture(data);
                tex_map_.emplace(id, i);
                return gl::material{ tex_pool_[i].unit(), depth };
            }
        }

        int depth = tex_pool_.emplace_back(format, width, height).create_texture(data);
        tex_map_.emplace(id, tex_pool_.size() - 1);
        return gl::material{ tex_pool_.back().unit(), depth };
    }
}

#endif