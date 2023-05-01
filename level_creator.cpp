#include "core/gore.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "signal/signal_layer.h"
#include "core/window.h"
#include "signal/signal_queue.h"
#include "core/timer.h"
#include "gl/program.h"
#include "util/level_editor.h"
#include "memory/renderable_storage.h"
#include "memory/texture_storage.h"
#include "memory/program_storage.h"
#include "core/input.h"
#include "scene/camera.h"
#include "core/renderer.h"

#include "player/player.h"
#include "state/game_state.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <thread>
#include <future>
#include <cstring>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <array>
#include "test_instrumentor.h"
#include "util/platform.h"
#include "scene/model.h"
#include "scene/static_model.h"
//#include "stb_image/stb_image.h"
#include "core/input.h"
#include "util/debug.h"
#include "gl/vertex_array.h"
#include "gl/program.h"
#include "glm/vec3.hpp"
#include "util/file_system.h"
#include "memory/buffer_storage.h"
#include "memory/render_data_storage.h"

int main()
{
	gore::signal_layer layer;
	gore::window wnd;
    gore::input inpt;
	gore::signal_queue queue;
    gore::util::platform pltfrm;
    //gore::util::platform::initialize_platform(); 
    gore::texture_storage tex_manager;
    gore::program_storage prg_manager;
    gore::renderable_storage ren_manager;
    gore::buffer_storage buf_manager;
    gore::render_data_storage rnd_manager;
	gore::util::level_editor editor;
	//gore::renderer rnd;
	
    gore::game_state gm_stt(new gore::in_editor);
    blood::player prkcz;

	gore::timer clck;

    const auto mdl = ren_manager.get_resource_id("self.obj");

    std::ifstream vertex("data/shaders/basic_vs.glsl");
	std::ifstream fragment("data/shaders/basic_fs.glsl");
	std::stringstream v_ss;
	std::stringstream f_ss;
	v_ss << vertex.rdbuf();
	f_ss << fragment.rdbuf();
	std::string vs_s = v_ss.str();
	std::string fs_s = f_ss.str();
	const char *vertex_shader = vs_s.c_str();
	const char *fragment_shader = fs_s.c_str();

    gore::gl::shader shdr(vertex_shader, gore::gl::shader_type::vertex);
    gore::gl::shader shdr2(fragment_shader, gore::gl::shader_type::fragment);
    gore::gl::program prg;
    prg.attach(&shdr);
    prg.attach(&shdr2);
    //prg.parse_program();
    prg.finalize();

    std::cout << "asd";
    return 0;

#if 0

	constexpr int how_many = 400;
    constexpr int vert_count = 3;
    constexpr int state = 1000;

    unsigned int vao, vbo, ssbo;
    glCreateVertexArrays(1, &vao);

    glEnableVertexArrayAttrib(vao, 0);
    glVertexArrayAttribBinding(vao, 0, 0);
    glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, 0);

    glEnableVertexArrayAttrib(vao, 1);
    glVertexArrayAttribBinding(vao, 1, 0);
    glVertexArrayAttribFormat(vao, 1, 2, GL_FLOAT, GL_FALSE, 4 * 3);

    glCreateBuffers(1, &vbo);
    glNamedBufferStorage(vbo, vert_count * sizeof(float) * 5 * how_many, nullptr, GL_DYNAMIC_STORAGE_BIT);

    glCreateBuffers(1, &ssbo);
    glNamedBufferStorage(ssbo, 3 * 4 * how_many, nullptr, GL_DYNAMIC_STORAGE_BIT | GL_MAP_READ_BIT);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, ssbo);

    int under = 0;
    int aside = 0;
    int first[how_many];
    int count[how_many];
    for (size_t i = 0; i < how_many; i++)
    {
        aside = i % 20;
        if (i % 20 == 0)
            under++;
        float vertices[] = {
            1.00f - (aside * 0.1f), 1.00f - (under * 0.1f), 0.0f, 1.0f, 1.0f,// top right
            0.90f - (aside * 0.1f), 0.90f - (under * 0.1f), 0.0f, 1.0f, 0.0f,// bottom right
            1.00f - (aside * 0.1f), 0.90f - (under * 0.1f), 0.0f, 0.0f, 1.0f,// top left                                         
        };
        float colors[] = {
            0.0f + static_cast<float>(i) / how_many,
            0.0f + static_cast<float>(i) / how_many,
            0.0f + static_cast<float>(i) / how_many
        };

        glNamedBufferSubData(vbo, i * vert_count * sizeof(float) * 5, vert_count * sizeof(float) * 5, vertices);
        glNamedBufferSubData(ssbo, i * sizeof(colors), sizeof(colors), colors);
        first[i] = i * 3;
        count[i] = 3;
    }

    glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_LEQUAL);

#endif

#if 0

    gore::static_model *mdl = res_manager.get_static_model("backpack.obj");

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load("C:\\project_gore\\data\\assets\\diffuse.jpg", &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);

#endif

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_PRIMITIVE_RESTART_FIXED_INDEX);
    glDepthFunc(GL_LEQUAL);
    glLineWidth(5.0f);

    int i = 0;
	while (!wnd.should_close() && !gore::input::is_key_in_state(gore::interrupts::e))
	{
        
		clck.frame();
        gm_stt.update();

        prkcz.update(clck.delta());

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		//rnd.render(prkcz, editor);

#if 0
        gore::gl::program &pr = res_manager.get_program(gore::program_type::basic);
        pr.bind();
        pr.mat4("view", cam.view());
        pr.mat4("projection", cam.projection());
		glBindVertexArray(vao);
        glVertexArrayVertexBuffer(vao, 0, vbo, 0, 4 * 5);
		//glMultiDrawArrays(GL_TRIANGLES, first, count, how_many);
#endif

		//for debug will change

#if 0
		gore::gl::program &pr = res_manager.get_program(gore::program_type::basic);
        gore::gl::vertex_array &vao = res_manager.get_vertex_format(gore::format_type::model);
        pr.bind();
        pr.mat4("view", cam.view());
        pr.mat4("projection", cam.projection());
        vao.bind();
        vao.bind_buffer(mdl->index_buffer(), mdl->vertex_buffer());

        int a = 0;
        for (const auto &msh : *mdl)
        {
            glDrawElements(GL_TRIANGLES, msh.index_size() / 4, GL_UNSIGNED_INT, (void*)a);
            a += msh.index_size();
        }
#endif

        inpt.update();
		wnd.update();
		queue.handle_all(clck.delta());
		wnd.swap();
        editor.update();

        using namespace std::chrono_literals;
        std::this_thread::sleep_for(0ms);
	}

	return 0;
}