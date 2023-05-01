#ifndef GORE_AREA_H
#define GORE_AREA_H

#include "scene/model_instance.h"
#include "physics/collision_body.h"
#include "scene/transform.h"
#include "util/platform.h"

#include <vector>
#include <string_view>

namespace gore
{
	class area : public spatial
	{
	public:
		void add_model(model_instance *model);
		void add_body(physics::collision_body *body);
	private:
		std::vector<model_instance*> models_;
		std::vector<physics::collision_body*> bodies_;
	};

	__FORCE_INLINE void area::add_model(model_instance *model)
    {
        models_.push_back(model);
		add_child(model);
    }
	__FORCE_INLINE void area::add_body(physics::collision_body *body)
    {
        bodies_.push_back(body);
		add_child(body);
    }
}

#endif