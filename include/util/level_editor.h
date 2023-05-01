#ifndef GORE_LEVEL_EDITOR_H
#define GORE_LEVEL_EDITOR_H

#include "scene/portal.h"
#include "memory/renderable_storage.h"
#include "memory/program_storage.h"
#include "scene/area.h"
#include "gl/program.h"
#include "scene/scene.h"
#include "scene/static_model.h"
#include "gl/dynamic_buffer.h"
#include "physics/static_body.h"
#include "scene/portal.h"

#include <string_view>
#include <vector>
#include <string>
#include <array>
#include <tuple>

namespace gore
{
	class renderer;

	namespace util
	{
		class level_editor
		{
		public:
			enum type
			{
				empty = 0,
				md_static,
				bd_static,
			};
			static constexpr const char* type_names[3]{
				"EMPTY",
				"STATIC_MODEL",
				"STATIC_BODY"
			};

			level_editor();
			void add_static_model(std::string_view path);
			void add_static_body(std::string_view path);

			void delete_model(std::string_view path);

			std::string get_description(std::tuple<size_t, type, size_t> pr)
			{
				return get_description(std::get<0>(pr), std::get<1>(pr));
			}
			std::string get_description(size_t index, type typ)
			{
				std::string name = model_list_[index];
				std::string type = type_names[typ];

				return (name + "/" + type).c_str();
			}

			void make_portal();
			void make_area();
			void prepare_scene(renderer &renderer_target);
			void update()
			{
				//db.swap();
				//deb.swap();
			}
			std::vector<std::string> model_list_;
			std::vector<std::tuple<size_t, type, size_t>> models_in_scene_;

			//model string index, model type, model storage index
			std::tuple<size_t, type, size_t> model_in_adjust_;

			int current_model_;
			int model_type_;
			bool model_added_;
			bool model_selected;

			std::vector<model_instance*> static_models_;
			std::vector<physics::collision_body*> static_bodies_;
		private:
			std::vector<gl::program> programs_;
			//aabb debug draw
			//gl::dynamic_buffer db;
			std::vector<portal> portals_;
		};

	}
}

#endif