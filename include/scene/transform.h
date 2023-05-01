#ifndef GORE_TRANSFORM_H
#define GORE_TRANSFORM_H

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtx/quaternion.hpp"

#include "scene/aabb.h"
#include "util/platform.h"

namespace gore
{
    struct transform //TODO transport the setting and getting function here
    {
        transform() = default;
        transform(const transform &trans) = default;
        transform(transform &&trans) = default;
        transform& operator=(const transform &trans) = default;
        transform& operator=(transform &&trans) = default;

        friend transform operator*(const transform &trans1, const transform &trans2); 

        glm::mat4 matrix();
        
        glm::vec3 pos_{ 0.0f, 0.0f, 0.0f };
        glm::vec3 scale_{ 1.0f, 1.0f, 1.0f };
        glm::vec3 rotation_{ 0.0f, 0.0f, 0.0f };
    };

    __FORCE_INLINE transform operator*(const transform &trans1, const transform &trans2)
    {
        transform trans;
        trans.pos_ = trans1.pos_ + trans2.pos_;
        trans.scale_ = trans1.scale_ * trans2.scale_;
        trans.rotation_ = trans1.rotation_ + trans2.rotation_;

        return trans;
    }
    __FORCE_INLINE glm::mat4 transform::matrix()
    {
        return glm::translate(glm::mat4(1.0f), pos_) *
               glm::toMat4(glm::quat(rotation_)) *
		       glm::scale(glm::mat4(1.0f), scale_);

        /*                                  REPR OF MATRIX
        std::cout << "  0 1 2 3\n";
        std::cout << "0 " << trans[0][0] << ' ' << trans[0][1] << ' ' << trans[0][2] << ' ' << trans[0][3] <<  '\n';
        std::cout << "1 " << trans[1][0] << ' ' << trans[1][1] << ' ' << trans[1][2] << ' ' << trans[1][3] << '\n';
        std::cout << "2 " << trans[2][0] << ' ' << trans[2][1] << ' ' << trans[2][2] << ' ' << trans[2][3] << '\n';
        std::cout << "3 " << trans[3][0] << ' ' << trans[3][1] << ' ' << trans[3][2] << ' ' << trans[3][3] << '\n';
        std::cout << "---------\n";*/
    }
}

#endif