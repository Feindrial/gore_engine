#ifndef GORE_RENDER_DATA_STORAGE_H
#define GORE_RENDER_DATA_STORAGE_H

#include "util/platform.h"

#include <unordered_map>

namespace gore
{
    struct render_data
    {
        unsigned int index_buffer_;
		unsigned int vertex_buffer_;
		unsigned int index_buffer_offset_;
        unsigned int vertex_buffer_offset_;
		unsigned int index_count_ = 0;
    };

    class render_data_storage
    {
    public:
        render_data_storage();
        ~render_data_storage();
        static render_data_storage* get() { return singleton_; }
        void reserve_render_data(int id);
        void set_buffers(int id, unsigned int vertex_buffer, unsigned int vertex_buffer_offset,
                                 unsigned int index_buffer, unsigned int index_buffer_offset);
        void set_vertex_buffer(int id, unsigned int vertex_buffer, unsigned int vertex_buffer_offset);
        void set_index_buffer(int id, unsigned int index_buffer, unsigned int index_buffer_offset);
        void increment_index_count(int id, unsigned int count);
        std::pair<unsigned int, unsigned int> get_reserved_locations(int id) const;
    private:
        std::unordered_map<int, render_data> render_data_;
        static render_data_storage *singleton_;
    };

    __FORCE_INLINE void render_data_storage::reserve_render_data(int id)
    {
        render_data_.try_emplace(id);
    }
    __FORCE_INLINE void render_data_storage::set_buffers(int id, unsigned int vertex_buffer, unsigned int vertex_buffer_offset,
                                                                 unsigned int index_buffer, unsigned int index_buffer_offset)
    {
        set_vertex_buffer(id, vertex_buffer, vertex_buffer_offset);
        set_index_buffer(id, index_buffer, index_buffer_offset);
    }
    __FORCE_INLINE void render_data_storage::set_vertex_buffer(int id, unsigned int vertex_buffer, unsigned int vertex_buffer_offset)
    {
        if (auto it = render_data_.find(id); it == render_data_.end())
            reserve_render_data(id);
        else
        {
            it->second.vertex_buffer_ = vertex_buffer;
            it->second.vertex_buffer_offset_ = vertex_buffer_offset;
        }
    }
    __FORCE_INLINE void render_data_storage::set_index_buffer(int id, unsigned int index_buffer, unsigned int index_buffer_offset)
    {
        if (auto it = render_data_.find(id); it == render_data_.end())
            reserve_render_data(id);
        else
        {
            it->second.index_buffer_ = index_buffer;
            it->second.index_buffer_offset_ = index_buffer_offset;
        }
    }
    __FORCE_INLINE void render_data_storage::increment_index_count(int id, unsigned int count)
    {

    }
    __FORCE_INLINE std::pair<unsigned int, unsigned int> render_data_storage::get_reserved_locations(int id) const
    {
        auto it = render_data_.find(id);
        return std::make_pair(it->second.vertex_buffer_offset_, it->second.index_buffer_offset_);
    }
}

#endif