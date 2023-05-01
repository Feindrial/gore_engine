#include "scene/camera.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "core/window.h"
#include "core/input.h"

namespace gore
{
    camera::camera() : pos_{ new glm::vec3{ 0.0f, 0.0f, 3.0f } }, front_{0.0f, 0.0f, -1.0f}, up_{0.0f, 1.0f, 0.0f},
        projection_{glm::perspective(glm::radians(45.0f), window::active().width() / window::active().height(), 0.1f, 100.0f)},
        view_{ glm::lookAt(*pos_, *pos_ + front_, up_) },
        lastx_{ 500.0f }, lasty_{ 500.0f }, yaw_{ -90.0f }, pitch_{ 0.0f }
    {
        signal_layer::get()->add_handler(std::bind(&camera::mouse_moved, this, std::placeholders::_1), time_based_signal::mouse_moved);
    }
    camera::camera(glm::vec3 *pos) : pos_{ pos }, front_{0.0f, 0.0f, -1.0f}, up_{0.0f, 1.0f, 0.0f},
        projection_{glm::perspective(glm::radians(45.0f), window::active().width() / window::active().height(), 0.1f, 100.0f)},
        view_{ glm::lookAt(*pos_, *pos_ + front_, up_) },
        lastx_{ 500.0f }, lasty_{ 500.0f }, yaw_{ -90.0f }, pitch_{ 0.0f }
    {
        signal_layer::get()->add_handler(std::bind(&camera::mouse_moved, this, std::placeholders::_1), time_based_signal::mouse_moved);
    }
    void camera::update(double delta) noexcept
    {
        float speed = 2.5f * delta;
		if (gore::input::is_key_in_state(gore::interrupts::w))
			*pos_ += speed * front_;

		if (gore::input::is_key_in_state(gore::interrupts::s))
			*pos_ -= speed * front_;

		if (gore::input::is_key_in_state(gore::interrupts::a))
			*pos_ -= glm::normalize(glm::cross(front_, up_)) * speed;

		if (gore::input::is_key_in_state(gore::interrupts::d))
			*pos_ += glm::normalize(glm::cross(front_, up_)) * speed;

        view_ = glm::lookAt(*pos_, *pos_ + front_, up_);
    }
}