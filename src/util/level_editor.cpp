#include "util/level_editor.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "core/window.h"
#include "util/platform.h"
#include "util/util.h"
#include "core/renderer.h"
#include "scene/static_model.h"
#include "physics/static_body.h"

#include <string_view>
#include <filesystem>
#include <vector>
#include <thread>

namespace gore
{
	namespace util
	{
		level_editor::level_editor() : model_in_adjust_{ 0ul, type::empty, 0ul }, current_model_{ 0 }, model_type_{ 0 }, model_added_{ false }
		{
			IMGUI_CHECKVERSION();
	    	ImGui::CreateContext();
	    	ImGuiIO &io = ImGui::GetIO(); (void)io;
	    	ImGui::StyleColorsDark();
	    	ImGui_ImplGlfw_InitForOpenGL(window::active().get(), true);
    		ImGui_ImplOpenGL3_Init("#version 450");

        	for (const auto &entry : std::filesystem::directory_iterator{ util::platform::exe_path_ / "data" / "assets" })
        	{
            	std::string filename = entry.path().filename().string();
            	if (filename.ends_with(".obj"))
                	model_list_.emplace_back(filename);
        	}			
		}
		void level_editor::add_static_model(std::string_view path)
		{
			//static_models_.emplace_back(renderable_storage::get()->get_static_model(path));
		}
		void level_editor::add_static_body(std::string_view path)
		{
			static_bodies_.emplace_back(new physics::static_body{ path });
		}
		void level_editor::prepare_scene(renderer &renderer_target) 
		{
			// gl::program *prg = program_storage::get()->get_program(program_type::basic);
			// gl::vertex_array *format = format_storage::get()->get_format(format_type::model);
			// for (model_instance *model : static_models_)
			// {
			// 	//renderer_target.push_state(prg, format, gl::draw_type::triangles, model);

			// 	aabb ab = model->get_transformed_aabb();
			// 	auto p_and_l = util::get_cube(ab.center_, ab.extend_);

			// 	unsigned int v_offset = db.push(p_and_l.first.data(), p_and_l.first.size() * sizeof(p_and_l.first[0]));
			// 	unsigned int e_offset = deb.push(p_and_l.second.data(), p_and_l.second.size() * sizeof(p_and_l.second[0]));

			// 	gl::program *deb_prg = program_storage::get()->get_program(program_type::debug);
			// 	gl::vertex_array *deb_format = format_storage::get()->get_format(format_type::debug);

			// 	renderer_target.push_state(deb_prg, deb_format, gl::draw_type::line_loop, render_data{
			// 		deb.data(),
			// 		db.data(),
			// 		e_offset / (unsigned int)sizeof(p_and_l.second[0]),
			// 		v_offset / (unsigned int)sizeof(p_and_l.first[0]),
			// 		p_and_l.second.size()
			// 	});
			// }	

			// for (physics::collision_body *body : static_bodies_)
			// {


			// 	renderer_target.push_state(prg, format, gl::draw_type::triangles, body->resource_);

			// 	for (physics::box_collider &shape : body->get_shapes())
			// 	{
			// 		auto p_and_l = util::get_cube(shape.size_, shape.get_global_transform().matrix());

			// 		unsigned int v_offset = db.push(p_and_l.first.data(), p_and_l.first.size() * sizeof(p_and_l.first[0]));
			// 		unsigned int e_offset = deb.push(p_and_l.second.data(), p_and_l.second.size() * sizeof(p_and_l.second[0]));

			// 		gl::program *deb_prg = program_storage::get()->get_program(program_type::debug);
			// 		gl::vertex_array *deb_format = format_storage::get()->get_format(format_type::debug);

			// 		renderer_target.push_state(deb_prg, deb_format, gl::draw_type::line_loop, render_data{
			// 			deb.data(),
			// 			db.data(),
			// 			e_offset / (unsigned int)sizeof(p_and_l.second[0]),
			// 			v_offset / (unsigned int)sizeof(p_and_l.first[0]),
			// 			p_and_l.second.size()
			// 		});
			// 	}

			// }
		}
	}
}