#include "core/game_state.h"

#include "core/input.h"
#include "core/timer.h"

namespace gore
{
    game_state::game_state(state *st) : current_state_{ st }
    {

    }

    state* in_game::update()
    {
        timer::time_scale(1.0f);
        if (gore::input::is_key_pushed(gore::interrupts::escape))
        {
            return new in_editor;
        }
        return nullptr;
    }

    state* in_editor::update()
    {
        timer::time_scale(0.0f);
        if (gore::input::is_key_pushed(gore::interrupts::escape))
        {
            return new in_game;
        }
        return nullptr;
    }
}