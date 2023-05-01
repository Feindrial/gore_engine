#ifndef GORE_RENDERER_H
#define GORE_RENDERER_H

#include "scene/scene.h"
#include "signal/signal_types.h"
#include "signal/signal_layer.h"
#include "signal/signal_queue.h"
#include "gl/program.h"
#include "gl/vertex_array.h"
#include "scene/static_model.h"
#include "scene/camera.h"
#include "memory/renderable_storage.h"
#include "player/player.h"

#include <array>
#include <vector>
#include <unordered_map>
#include <set>

namespace gore
{
	namespace util
	{
		class level_editor;
	}

	class renderer
	{
	public:
		renderer();
		void render(const scene &scn);
		void render(const blood::player& plyr, util::level_editor &lvl_editor);
	private:

		struct render_state
		{
			
		};

		struct target_state
		{
			gl::program *state_;
			std::vector<unsigned int> children_;
		};
		struct format_state
		{
			gl::vertex_array *state_;
			std::vector<unsigned int> children_;
		};

		struct buffer_state
		{
			unsigned int eb_state_;
			unsigned int vb_state_;
			std::vector<unsigned int> ins_children_;
			std::vector<unsigned int> non_ins_children_;
		};

		struct instance;
		struct instanced_draw_state
		{
			std::vector<unsigned int> instances() const
			{
				std::vector<unsigned int> inst;
				for (const instance &ins : instances_) { inst.push_back(ins.models_.size()); } 
				return inst;
			}

			//gl::draw_type type_;
			char state_id_;
			std::vector<instance> instances_;
		};

		struct non_instanced_draw_state
		{
			unsigned int count_ = 1;
			//gl::draw_type type_;
			char state_id_;
		};

		struct instance
		{
			instance(model_instance *mdl) : models_{ mdl } {}
			bool is_same(model_instance *mdl) { return *models_[0] == *mdl; }

			std::vector<model_instance*> models_;
		};

		void scene_graph(spatial *sptl);
		void model_push(signal *st);
		void set_state() const;

		//state_data states_;
		std::vector<target_state> prog_state_;
		std::vector<format_state> format_state_;
		std::vector<buffer_state> buffer_state_;
		std::vector<instanced_draw_state> instanced_draw_state_;
		std::vector<non_instanced_draw_state> non_instanced_draw_state_; 
		char state_counter_;
	};

	inline void renderer::model_push(signal *st)
	{
		st->handled_ = true;
		auto *sgnl = dynamic_cast<model_push_signal*>(st);
		sgnl->thread_.join();
	}
}

#endif