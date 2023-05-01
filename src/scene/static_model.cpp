#include "scene/static_model.h"

#include "memory/renderable_storage.h"

#include <string_view>

namespace gore
{
    static_model::static_model(std::string_view path) : 
        model_instance{ renderable_storage::get()->get_resource_id(path, resource_hint::static_resource) }
    {
        
    }
    static_model::static_model(int resource) : model_instance{ resource }
    {
        
    }
}