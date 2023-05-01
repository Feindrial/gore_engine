#include "memory/renderable_storage.h"

#include "scene/static_model.h"
#include "util/file_system.h"

#include <string_view>
#include <array>

namespace gore
{
    renderable_storage *renderable_storage::singleton_ = nullptr;

    renderable_storage::renderable_storage()
    {
        singleton_ = this;
    }
    renderable_storage::~renderable_storage()
    {

    }
}