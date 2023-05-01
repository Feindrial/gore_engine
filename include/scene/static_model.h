#ifndef GORE_STATIC_MODEL_H
#define GORE_STATIC_MODEL_H

#include "scene/model.h"
#include "scene/model_instance.h"
#include "scene/aabb.h"

#include <string_view>
#include <iostream>
namespace gore
{
    class static_model : public model_instance
    {
    public:
        static_model() = default;
        explicit static_model(std::string_view path);
        explicit static_model(int resource);
        //bool is_collided(const gore::model_instance *mdl) const;
    }; 

    // inline bool static_model::is_collided(const gore::model_instance *mdl) const
	// {
    //     aabb mdlab = mdl->get_transformed_aabb();

    //     //aabb debug
    //     // auto pr = [](const gore::aabb &ab, const std::string &who) { 
    //     //     std::cout << who << "-> (min) x: " << ab.min_.x << " y: " << ab.min_.y << " z: " << ab.min_.z << '\n';
    //     //     std::cout << who << "-> (max) x: " << ab.max_.x << " y: " << ab.max_.y << " z: " << ab.max_.z << '\n';
    //     // };
    //     // pr(get_transformed_aabb(), "plyr");
    //     // pr(mdlab, "extrnal");

	// 	if (get_transformed_aabb().intersect(mdlab))
	// 	{
	// 		std::cout << "player is collided" << '\n';
	// 		return true;
	// 	}
    //     std::cout << "not collided" << '\n';

	// 	return false;
	// }
}

#endif