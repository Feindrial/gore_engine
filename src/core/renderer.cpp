#include "core/renderer.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/trigonometric.hpp"
#include "glm/vec3.hpp"

#include "util/level_editor.h"
#include "util/platform.h"
#include "memory/renderable_storage.h"
#include "scene/static_model.h"
#include "scene/camera.h"
#include "signal/signal_types.h"
#include "gl/program.h"
#include "gl/vertex_array.h"
#include "scene/light.h"
#include "player/player.h"

#include <filesystem>
#include <functional>
#include <vector>
#include <array>
#include <initializer_list>
#include <string>
#include <algorithm>

namespace gore
{
    renderer::renderer() : state_counter_{ 0 }
    {
        signal_layer::get()->add_handler(std::bind(&renderer::model_push, this, std::placeholders::_1),
                            non_time_based_signal::model_push);
    }
    void renderer::render(const blood::player& plyr, util::level_editor &lvl)
    {
        ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
       //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        //scene draw

        //prepare scene

        lvl.prepare_scene(*this); //will fix this design issue later

        signal_queue::get()->handle(non_time_based_signal::model_push);

        //light test (will be uniform block later)


        //draw scene;
        int i = 0;
        for (const target_state &t_state : prog_state_)
        { i = (i + 1) % 360;
            t_state.state_->bind();
            //t_state.state_->mat4("mvp", plyr.mvp());
            t_state.state_->vec3("view_pos_", plyr.pos());



            for (unsigned int t_child : t_state.children_)
            {
                format_state_[t_child].state_->bind();

                for (unsigned int f_child : format_state_[t_child].children_)
                {

                    for (unsigned int b_child : buffer_state_[f_child].non_ins_children_)
                    {
                    format_state_[t_child].state_->bind_buffer(buffer_state_[f_child].eb_state_,
                                                               buffer_state_[f_child].vb_state_);
                        rnd_buffer_.draw(non_instanced_draw_state_[b_child].state_id_,
                                         non_instanced_draw_state_[b_child].count_,
                                         non_instanced_draw_state_[b_child].type_);
                    }


                    for (unsigned int b_child : buffer_state_[f_child].ins_children_)
                    {
                    format_state_[t_child].state_->bind_buffer(buffer_state_[f_child].eb_state_,
                                                               buffer_state_[f_child].vb_state_,
                                                               counter_.data());
                        unsigned int counter = 0;
                        for (instance &ins : instanced_draw_state_[b_child].instances_)
                        {
                            for (model_instance *mdl : ins.models_)
                            {
                                counter_.push(counter);

                                ssbo_.push(gl::independent_uniform_data{
                                    mdl->get_global_transform().matrix(),
                                    //mdl->material()
                                });

                                //collision query
                                //plyr.is_collided(mdl);
                            }
                            counter += ins.models_.size();
                        }
                        ssbo_.bind();

                        rnd_buffer_.draw(instanced_draw_state_[b_child].state_id_,
                                         instanced_draw_state_[b_child].instances_.size(),
                                         instanced_draw_state_[b_child].type_,
                                         instanced_draw_state_[b_child].instances());

                    }
                }
            }
        }
       
        prog_state_.clear();
        format_state_.clear();
        buffer_state_.clear();
        instanced_draw_state_.clear();
        non_instanced_draw_state_.clear();
        state_counter_ = 0;

        counter_.swap();
        ssbo_.swap();
        rnd_buffer_.swap();

        //gui draw
      

		ImGui::ShowDemoWindow();

        ImGui::Begin("Scene");

        if (ImGui::BeginListBox("Model List"))
        {
            for (size_t i = 0; i < lvl.model_list_.size(); i++)
            {
                const bool selected = lvl.current_model_ == i;
                if (ImGui::Selectable(lvl.model_list_[i].c_str(), selected))
                    lvl.current_model_ = i;

                if (selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndListBox();
        }

        using mdl = util::level_editor::type;
        ImGui::RadioButton("Static Model", &lvl.model_type_, mdl::md_static); ImGui::SameLine();
        ImGui::RadioButton("Static Body", &lvl.model_type_, mdl::bd_static);

        if (ImGui::Button("Add Model to Scene"))
            lvl.model_added_ = true;

        if (lvl.model_added_)
        {
            switch (lvl.model_type_)
            {
            case mdl::md_static:
                lvl.models_in_scene_.emplace_back(lvl.current_model_, mdl::md_static, lvl.static_models_.size());
                lvl.add_static_model(lvl.model_list_[lvl.current_model_]);
                
                break;
            case mdl::bd_static:
                lvl.models_in_scene_.emplace_back(lvl.current_model_, mdl::bd_static, lvl.static_bodies_.size());
                lvl.add_static_body(lvl.model_list_[lvl.current_model_]);

                break;

            default:
                break;
            }

            lvl.model_added_ = false; //logic error but doesnt matter
        }

        if (ImGui::BeginListBox("In the Scene"))
        {
            for (size_t i = 0; i < lvl.models_in_scene_.size(); i++)
            {
                const bool selected = std::get<2>(lvl.model_in_adjust_) == i;
                if (ImGui::Selectable((std::to_string(i) + " - " + 
                                       lvl.get_description(std::get<0>(lvl.models_in_scene_[i]), std::get<1>(lvl.models_in_scene_[i]))).c_str(), selected))
                    lvl.model_in_adjust_ = lvl.models_in_scene_[i];

                if (selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndListBox();
        }

        ImGui::Text("SCENE GRAPH");

        // for (size_t i = 0; i < lvl.portals_.size(); i++)
        // {
        //     lvl.portals_[i]
        // }

        ImGui::End();

        ImGui::Begin("Model Settings");

        ImGui::Text(lvl.get_description(lvl.model_in_adjust_).c_str());
        
        switch (std::get<1>(lvl.model_in_adjust_))
        {
        case mdl::md_static:
            if (ImGui::CollapsingHeader("TRANSFORMATIONS"))
            {
                glm::vec3 pos = lvl.static_models_[std::get<2>(lvl.model_in_adjust_)]->get_translate();
                glm::vec3 scl = lvl.static_models_[std::get<2>(lvl.model_in_adjust_)]->get_scale();
                glm::vec3 rot = lvl.static_models_[std::get<2>(lvl.model_in_adjust_)]->get_rotate();

                ImGui::SliderFloat3("Position", &pos.x, -10.0f, 10.0f);
                ImGui::SliderFloat3("Scale", &scl.x, 0.0f, 10.0f);
                
                ImGui::SliderFloat("XAngle", &rot.x, glm::radians(-180.0f), glm::radians(180.0f));
                ImGui::SliderFloat("YAngle", &rot.y, glm::radians(-180.0f), glm::radians(180.0f));
                ImGui::SliderFloat("ZAngle", &rot.z, glm::radians(-180.0f), glm::radians(180.0f));

                lvl.static_models_[std::get<2>(lvl.model_in_adjust_)]->set_translate(pos);
                lvl.static_models_[std::get<2>(lvl.model_in_adjust_)]->set_scale(scl);
                lvl.static_models_[std::get<2>(lvl.model_in_adjust_)]->set_rotation(rot);
            }
            break;
        
        case mdl::bd_static:
            if (ImGui::CollapsingHeader("TRANSFORMATIONS"))
            {
                glm::vec3 pos = lvl.static_bodies_[std::get<2>(lvl.model_in_adjust_)]->get_translate();
                glm::vec3 scl = lvl.static_bodies_[std::get<2>(lvl.model_in_adjust_)]->get_scale();
                glm::vec3 rot = lvl.static_bodies_[std::get<2>(lvl.model_in_adjust_)]->get_rotate();

                ImGui::SliderFloat3("Position", &pos.x, -10.0f, 10.0f);
                ImGui::SliderFloat3("Scale", &scl.x, 0.0f, 10.0f);
                
                ImGui::SliderFloat("XAngle", &rot.x, glm::radians(-180.0f), glm::radians(180.0f));
                ImGui::SliderFloat("YAngle", &rot.y, glm::radians(-180.0f), glm::radians(180.0f));
                ImGui::SliderFloat("ZAngle", &rot.z, glm::radians(-180.0f), glm::radians(180.0f));

                lvl.static_bodies_[std::get<2>(lvl.model_in_adjust_)]->set_translate(pos);
                lvl.static_bodies_[std::get<2>(lvl.model_in_adjust_)]->set_scale(scl);
                lvl.static_bodies_[std::get<2>(lvl.model_in_adjust_)]->set_rotation(rot);


            }

            if (ImGui::CollapsingHeader("PHYSICS"))
            {

                static glm::vec3 added_box(0.0f);
                ImGui::Text("Add Shape to the Body");
                ImGui::SliderFloat3("Size", &added_box.x, 0.0f, 10.0f); ImGui::SameLine();
                if (ImGui::Button("Add"))
                    lvl.static_bodies_[std::get<2>(lvl.model_in_adjust_)]->add_shape(added_box);

                int i = 0;
                for (physics::box_collider &shp : lvl.static_bodies_[std::get<2>(lvl.model_in_adjust_)]->get_shapes())
                {
                    if (ImGui::TreeNode(("CollisionShape" + std::to_string(i)).c_str()))
                    {
                        ImGui::SliderFloat3("Size", &shp.size_.x, 0.0f, 10.0f);

                        glm::vec3 shp_pos = shp.get_translate();
                        glm::vec3 shp_scl = shp.get_scale();
                        glm::vec3 shp_rot = shp.get_rotate();

                        ImGui::SliderFloat3("Position", &shp_pos.x, -10.0f, 10.0f);
                        ImGui::SliderFloat3("Scale", &shp_scl.x, 0.0f, 10.0f);

                        ImGui::SliderFloat("XAngle", &shp_rot.x, glm::radians(-180.0f), glm::radians(180.0f));
                        ImGui::SliderFloat("YAngle", &shp_rot.y, glm::radians(-180.0f), glm::radians(180.0f));
                        ImGui::SliderFloat("ZAngle", &shp_rot.z, glm::radians(-180.0f), glm::radians(180.0f));

                        shp.set_translate(shp_pos);
                        shp.set_scale(shp_scl);
                        shp.set_rotation(shp_rot);

                        ImGui::TreePop();
                    }

                    if (ImGui::Button("Delete the Shape"))
                        lvl.static_bodies_[std::get<2>(lvl.model_in_adjust_)]->delete_shape(i);

                    i++;
                }    
            }
            break;

        default:
            break;
        }
      

        ImGui::Text("Press ESC to switch between in-game and editor.");
        ImGui::Text("Press E to close the game/editor.");
        ImGui::Text("WASD and Mouse will control camera.");

        ImGui::Text("Can't adjust multiple models in the same time so click for one only at a time.");

        ImGui::End();

        ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());     
    }
}