#ifndef GORE_LIGHT_H
#define GORE_LIGHT_H

#include "glm/vec3.hpp"

namespace gore
{
    struct light_data
    {
        glm::vec3 pos_;
        glm::vec3 ambient_;
        glm::vec3 diffuse_;
        glm::vec3 specular_;
    };

    class light
    {
    public:
        light(light_data data);
        glm::vec3 pos_;
        glm::vec3 ambient_;
        glm::vec3 diffuse_;
        glm::vec3 specular_;
    private:
    };
}

#endif