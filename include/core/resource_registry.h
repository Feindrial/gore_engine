#pragma once

#include "core/gresource.h"
#include "core/platform_macros.h"

#include <mutex>
#include <stack>
#include <vector>

namespace gore
{
    class resource_registry
    {
    public:
        void register_gresource(gresource *res);
        void unregister_gresource(gresource::gid id);
    private:
        std::mutex lock_;
        std::stack<gresource::gid, std::vector<gresource::gid>> free_ids_;
        gresource::gid next_gid_;
    };

    __FORCE_INLINE void resource_registry::register_gresource(gresource *res)
    {
        const std::lock_guard<std::mutex> lock(lock_);

        if (!free_ids_.empty())
        {
            res->gid_ = free_ids_.top();
            free_ids_.pop();
        }
        else
            res->gid_ = next_gid_++;
    }
    __FORCE_INLINE void resource_registry::unregister_gresource(gresource::gid id)
    {
        const std::lock_guard<std::mutex> lock(lock_);

        free_ids_.push(id);
    }
}