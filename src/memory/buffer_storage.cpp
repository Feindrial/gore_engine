#include "memory/buffer_storage.h"

#include "memory/render_data_storage.h"

#include <string>

namespace gore
{
    buffer_storage *buffer_storage::singleton_ = nullptr;

    buffer_storage::buffer_storage()
    {
        singleton_ = this;
    }
    buffer_storage::~buffer_storage()
    {
     
    }

    void buffer_storage::reserve_ubo_data(std::string name)
    {
        // for (size_t i = 0; i < ubos_.size(); i++)
        // {
        //     if (ubos_[i].room())
        //     {
        //         ubos_[i].reserve(name);
        //         ubo_map_.emplace(name, i);

        //         return;
        //     }
        // }
        // ubos_.emplace_back().reserve(name);
        // ubo_map_.emplace(name, ubos_.size() - 1);
    }
    void buffer_storage::reserve_buffer_data(int id, int vertex_length_hint, int index_length_hint, buffer_hint hint)
    {
        bool sb_push = true;
        for (size_t i = 0; i < static_buffers_.size(); i++)
        {
            if (static_buffers_[i].room() > vertex_length_hint)
            {
                unsigned int loc = static_buffers_[i].reserve(id, vertex_length_hint);
                sb_map_.emplace(id, i);

                render_data_storage::get()->set_vertex_buffer(id, static_buffers_[i].id(), loc);

                sb_push = false;
                break;
            }
        }
        if (sb_push)
        {
            unsigned int loc = static_buffers_.emplace_back().reserve(id, vertex_length_hint);
            sb_map_.emplace(id, static_buffers_.size() - 1);

            render_data_storage::get()->set_vertex_buffer(id, static_buffers_.back().id(), loc);
        }

        bool seb_push = true;
        for (size_t i = 0; i < static_element_buffers_.size(); i++)
        {
            if (static_element_buffers_[i].room() > index_length_hint)
            {
                unsigned int loc = static_element_buffers_[i].reserve(id, index_length_hint);
                seb_map_.emplace(id, i);

                render_data_storage::get()->set_index_buffer(id, static_element_buffers_[i].id(), loc);

                seb_push = false;
                break;
            }
        }
        if (seb_push)
        {
            unsigned int loc = static_element_buffers_.emplace_back().reserve(id, index_length_hint);
            seb_map_.emplace(id, static_element_buffers_.size() - 1);

            render_data_storage::get()->set_index_buffer(id, static_element_buffers_.back().id(), loc);
        }
    }
}