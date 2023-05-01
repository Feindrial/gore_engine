#ifndef GORE_CAMERA_H
#define GORE_CAMERA_H

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/trigonometric.hpp"

#include "signal/signal_layer.h"
#include "signal/signal_types.h"
#include "state/game_state.h"
#include "core/timer.h"
#include <iostream>
namespace gore
{
    class camera
    {
    public:
        camera();
        camera(glm::vec3 *pos);
        glm::mat4 view() const;
        glm::mat4 projection() const;
        void update(double delta) noexcept;
        void mouse_moved(signal *st);
        glm::vec3 *pos_;
    private:
        glm::vec3 front_;    
        glm::vec3 up_;
        glm::mat4 projection_;
        glm::mat4 view_;
        float lastx_;
        float lasty_;
        float yaw_;
        float pitch_;
        bool first_moved_;
        bool dirty_;
    };

    inline glm::mat4 camera::view() const
    {
        return view_;
    }
    inline glm::mat4 camera::projection() const
    {
        return projection_;
    }
    inline void camera::mouse_moved(signal *st)
    {
        mouse_moved_signal *signl = dynamic_cast<mouse_moved_signal*>(st);
        signl->handled_ = false;


        if (first_moved_)
        {
            lastx_ = signl->xpos_;
            lasty_ = signl->ypos_;
            first_moved_ = false;
        }

        if (signl->delta_ != 0.0)
        {
            float xoffset = (signl->xpos_ - lastx_) * 0.1f;
            float yoffset = (lasty_ - signl->ypos_) * 0.1f;
            lastx_ = signl->xpos_;
            lasty_ = signl->ypos_;
            yaw_ += xoffset;
            pitch_ += yoffset;

            if (pitch_ > 89.0f)
                pitch_ = 89.0f;
            if (pitch_ < -89.0f)
                pitch_ = -89.0f; 

            glm::vec3 front;
            front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
            front.y = sin(glm::radians(pitch_));
            front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
            front_ = glm::normalize(front);
        }
    }
}

#endif