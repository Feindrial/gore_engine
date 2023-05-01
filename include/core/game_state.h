#ifndef GORE_GAME_STATE_H
#define GORE_GAME_STATE_H

#include "core/window.h"
#include "util/platform.h"

namespace gore
{
    struct state
    {
    public:
        virtual state* update() = 0;
    };

    struct in_game final : public state
    {
    public:
        in_game() { glfwSetInputMode(gore::window::active().get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED); }
        state* update() override;
    };
    struct in_editor final : public state
    {
    public:
        in_editor() { glfwSetInputMode(gore::window::active().get(), GLFW_CURSOR, GLFW_CURSOR_NORMAL); }
        state* update() override;
    };
    
    class game_state
    {
    public:
        game_state(state *st);
        void update();
    private:
        state* current_state_;
    };

    __FORCE_INLINE void game_state::update()
    {
        state *state_changed = current_state_->update();

        if (state_changed)
        {
            delete current_state_;
            current_state_ = state_changed;
        }
    }
}

#endif