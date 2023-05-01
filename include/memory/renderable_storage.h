#ifndef GORE_RENDERABLE_STORAGE_H
#define GORE_RENDERABLE_STORAGE_H

#include "scene/model.h"
#include "gl/static_buffer.h"
#include "memory/texture_storage.h"
#include "gl/material.h"
#include "util/platform.h"

#include <vector>
#include <unordered_map>
#include <string>
#include <string_view>
#include <utility>

namespace gore
{
    using rid = int;

    enum class resource_hint
    {
        static_resource  = util::bit(0),
        dynamic_resource = util::bit(1)
    };

    class renderable_storage
    {
    public:
        renderable_storage();
        ~renderable_storage();
        static renderable_storage* get() { return singleton_; }

        rid get_resource_id(std::string_view path, resource_hint hint = resource_hint::static_resource);
        model& get_resource(std::string path);
        model& get_resource(rid id);
    private:
        std::vector<model> resources_;
        std::unordered_map<std::string, rid> resource_map_;
        static renderable_storage *singleton_;
    };

    __FORCE_INLINE rid renderable_storage::get_resource_id(std::string_view path, resource_hint hint)
    {
        const std::string &path_ = file_system::get_asset_path(path);
        auto rsrc = resource_map_.find(path_);

        if (rsrc != resource_map_.cend())
            return rsrc->second;
        else
        {
            rid id = resources_.size();
            resources_.emplace_back(path_, id);
            resource_map_.emplace(path_, id);
            return id;
        }
    }
    __FORCE_INLINE model& renderable_storage::get_resource(std::string path)
    {
        const std::string &path_ = file_system::get_asset_path(path);   
        auto rsrc = resource_map_.find(path_);

        if (rsrc != resource_map_.cend())
            return resources_[rsrc->second];
        else
        {
            //resource is not in memory
        }
    }
    __FORCE_INLINE model& renderable_storage::get_resource(rid id)
    {
        return resources_[id];
    }
}

#endif