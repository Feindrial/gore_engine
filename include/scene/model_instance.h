#ifndef GORE_MODEL_INSTANCE_H
#define GORE_MODEL_INSTANCE_H

#include "glm/mat4x4.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/ext/matrix_transform.hpp"

#include "scene/model.h"
#include "scene/aabb.h"
#include "scene/transform.h"
#include "scene/scene_visual.h"
#include "util/util.h"
#include "util/platform.h"
#include "memory/renderable_storage.h"

#include <string_view>
#include <array>
#include <cmath>

namespace gore
{
    class model_instance : public scene_visual
    {
    public:
        model_instance() = default;
        explicit model_instance(int resource);
        friend bool operator==(const model_instance &mdl1, const model_instance &mdl2);
        friend bool operator!=(const model_instance &mdl1, const model_instance &mdl2);
        aabb get_transformed_aabb();
    protected:
    private:
        int resource_;
    };

    __FORCE_INLINE bool operator==(const model_instance &mdl1, const model_instance &mdl2)
    {
        return mdl1.resource_ == mdl2.resource_;
    }
    __FORCE_INLINE bool operator!=(const model_instance &mdl1, const model_instance &mdl2)
    {
        return !(operator==(mdl1, mdl2));
    }
    __FORCE_INLINE aabb model_instance::get_transformed_aabb()
    {
        const aabb &rsrc_ab = renderable_storage::get()->get_resource(resource_).aabb_;
        aabb ab;
        const glm::mat4 &mtrx = get_global_transform().matrix();
        for (int i = 0; i < 3; i++)
        {
            ab.center_[i] = mtrx[3][i];
            ab.extend_[i] = 0.0f;
            for (int j = 0; j < 3; j++) 
            {
                ab.center_[i] += mtrx[i][j] * rsrc_ab.center_[j];
                ab.extend_[i] += std::abs(mtrx[i][j]) * rsrc_ab.extend_[j];
            }
        }
            // int i = 1;
            // if (resource_->aabb_.center_[i] > 0.0f)
            // {
            //     int axis1 = (i + 1) % 3;
            //     int axis2 = (i + 2) % 3;

            //     ab.extend_[axis1] -= resource_->aabb_.extend_[i] * glm::sin(trans_.rotation_[axis2]);
            //     ab.extend_[axis2] -= resource_->aabb_.extend_[i] * glm::sin(trans_.rotation_[axis1]);
            // }
            // ab.center_.x -= resource_->aabb_.extend_.y;
            
        return ab;
        // glm::vec3 min = matrix() * glm::vec4(resource_->aabb_.min_, 1.0f);
        // glm::vec3 max = matrix() * glm::vec4(resource_->aabb_.max_, 1.0f);
        // return aabb(min, max);
    }
}

#endif