#ifndef GORE_DYNAMIC_BUFFER_H
#define GORE_DYNAMIC_BUFFER_H

#include "glad/glad.h"

#include "gl/buffer.h"
#include "util/util.h"

#include <cstring>
#include <cmath>
#include <string>
#include <list>
#include <unordered_map>
#include <tuple>
#include <algorithm>

namespace gore
{
    namespace gl
    {
        template<typename T = int>
        class dynamic_buffer : public buffer
        {
            struct blob_data
            {
                unsigned int start_blob_;
                unsigned int end_blob_;
            };
        public:
            dynamic_buffer();
            dynamic_buffer(unsigned int size, util::information_size info);

            unsigned int reserve(T id, unsigned int length = blob_size_);
			void push(T id, const void *data, unsigned int length);
            void swap();
        protected:
            static constexpr unsigned int blob_size_ = 4 * util::information_size::kb;
            static constexpr unsigned int min_align_blob_num_ = 2;

            std::list<blob_data> free_list_;
            std::unordered_map<T, std::tuple<blob_data, unsigned int>> alloc_list_;
            char section_;
            void *map_;
        private:
        };

        template<typename T>
        dynamic_buffer<T>::dynamic_buffer() 
        {
            glNamedBufferStorage(id_, 3 * size_, nullptr, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
            map_ = glMapNamedBufferRange(id_, 0, 3 * size_,  GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);

            free_list_.emplace(0, size_ / blob_size_ - 1);
        }
        template<typename T>
        dynamic_buffer<T>::dynamic_buffer(unsigned int size, util::information_size info) :
            buffer{ size * info }
        {
            glNamedBufferStorage(id_, 3 * size_, nullptr, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
            map_ = glMapNamedBufferRange(id_, 0, 3 * size_,  GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);

            free_list_.emplace(0, size_ / blob_size_ - 1);
        }
        template<typename T>
        unsigned int dynamic_buffer<T>::reserve(T id, unsigned int length)
        {
            auto &data = free_list_.back();
            unsigned int loc = (data.end_blob_ - data.start_blob_) / 2;

            unsigned int bsize = std::ceil(length / blob_size_);

            blob_data left{ data.start_blob_, loc };
            blob_data right{ loc + bsize, data.end_blob_};

            alloc_list_.try_emplace(id, loc, loc + bsize - 1, 0);

            const auto flcomp = [](const blob_data &b1, const blob_data &b2){ return b1.start_blob_ < b2.start_blob_; };
            free_list_.emplace(std::upper_bound(free_list_.begin(), free_list_.end(), left, flcomp), left);
            free_list_.emplace(std::upper_bound(free_list_.begin(), free_list_.end(), right, flcomp), right);
            free_list_.pop_back();
        }
        template<typename T>
        inline void dynamic_buffer<T>::push(T id, const void *data, unsigned int length)
        {
            auto it = alloc_list_.find(id);

            if (std::get<blob_data>(it).end_blob_ - std::get<blob_data>(it).start_blob_ + 1 <= (std::get<unsigned int>(it) + length) / blob_size_)
            {
                // not enough mem
                const auto val = std::get<blob_data>(it).end_blob_ + 1;
                const auto needed = std::ceil((std::get<unsigned int>(it) + length) / blob_size_) - std::get<blob_data>(it).end_blob_;
                const auto flcomp = [&val, &needed](const blob_data &bd)
                { 
                    return bd.start_blob_ == val && (bd.end_blob_ - bd.start_blob_) >= needed + min_align_blob_num_;
                };

                if (auto fnd = std::find_if(free_list_.begin(), free_list_.end(), flcomp); fnd == free_list_.end())
                {
                    // reserve new mem but no aval must create new buffer
                }
                else
                {
                    // stretch the mem  
                    fnd->start_blob_ += needed;
                    std::get<blob_data>(it).end_blob_ += needed;
                }
            }

            std::memcpy((char*)map_ + section_ * size_ + (std::get<blob_data>(it).start_blob_ * blob_size_) + std::get<unsigned int>(it),
                        data, length);

            std::get<unsigned int>(it) += length;
        }
        template<typename T>
        inline void dynamic_buffer<T>::swap()
        {
            current_ = 0;
			section_ = (section_ + 1) % 3;
        }
    }
}

#endif