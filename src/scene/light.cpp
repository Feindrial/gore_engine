#include "scene/light.h"

namespace gore
{
    light::light(light_data data) : 
        pos_{ data.pos_ }, ambient_{ data.ambient_ }, diffuse_{ data.diffuse_ }, specular_{ data.specular_ }
    {
        
    }
}