#ifndef GORE_SPATIAL_H
#define GORE_SPATIAL_H

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"

#include "util/platform.h"
#include "util/util.h"
#include "scene/transform.h"
#include "scene/scene_node.h"

#include <vector>

namespace gore
{
    class spatial : public scene_node
    {
    public:
        spatial();
        explicit spatial(const transform &trans);

        transform get_global_transform();
        void set_rotation(const glm::vec3 &rot);
        void set_scale(const glm::vec3 &scl);
        void set_translate(const glm::vec3 &trns);
        void rotate(const glm::vec3 &rot);
        void scale(const glm::vec3 &scl);
        void translate(const glm::vec3 &trns);
        glm::vec3 get_rotate() const;
        glm::vec3 get_scale() const;
        glm::vec3 get_translate() const;
    private:
        void notify_dirty_transformation();
        transform global_trans_;
        transform local_trans_;
        bool dirty_;
    };

    __FORCE_INLINE transform spatial::get_global_transform()
    {
        if (dirty_)
        {
            if (parent_)
            {
                spatial *sptl = dynamic_cast<spatial*>(parent_);

                if (sptl)
                    global_trans_ = sptl->get_global_transform() * local_trans_;
                else
                    global_trans_ = local_trans_;
            }
            else
                global_trans_ = local_trans_;

            dirty_ = false;
        }

        return global_trans_;
    }
    __FORCE_INLINE void spatial::set_rotation(const glm::vec3 &rot)
    {
        local_trans_.rotation_ = rot;
        notify_dirty_transformation();
    }
    __FORCE_INLINE void spatial::set_scale(const glm::vec3 &scl)
    {
        local_trans_.scale_ = scl;
        notify_dirty_transformation();
    }
    __FORCE_INLINE void spatial::set_translate(const glm::vec3 &trns)
    {
        local_trans_.pos_ = trns;
        notify_dirty_transformation();
    }
    __FORCE_INLINE void spatial::rotate(const glm::vec3 &rot)
    {
        local_trans_.rotation_ += rot;
        notify_dirty_transformation();
    }
    __FORCE_INLINE void spatial::scale(const glm::vec3 &scl)
    {
        local_trans_.scale_ += scl;
        notify_dirty_transformation();
    }
    __FORCE_INLINE void spatial::translate(const glm::vec3 &trns)
    {
        local_trans_.pos_ += trns;
        notify_dirty_transformation();
    }
    __FORCE_INLINE glm::vec3 spatial::get_rotate() const
    {
        return local_trans_.rotation_;
    }
    __FORCE_INLINE glm::vec3 spatial::get_scale() const
    {
        return local_trans_.scale_;
    }
    __FORCE_INLINE glm::vec3 spatial::get_translate() const
    {
        return local_trans_.pos_;
    }
    __FORCE_INLINE void spatial::notify_dirty_transformation()
    {
        dirty_ = true;

        for (scene_node *node : children_)
        {
            spatial *sptl = dynamic_cast<spatial*>(node);

            if (sptl)
            {
                sptl->dirty_ = true;
                sptl->notify_dirty_transformation();
            }
        }
    }
}

#endif