#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "stb_image/stb_image.h"

#include <iostream>
#include <array>
#include <utility>
#include <algorithm>
#include <fstream>
#include <vector>
#include <iomanip>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings

#define data_stream_test 0
#define multiple_vbo 0
#define model_valid 0

const char* vertexShaderSource = "#version 450 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec2 texCoord;\n"
"layout (location = 2) in uint counter;\n"
"out vec2 TexCoord;\n"
"flat out uint cont;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"   TexCoord = texCoord;\n"
"   cont = counter;\n"
"}\0";
const char* fragmentShaderSource = "#version 450 core\n"
"out vec4 FragColor;\n"
"in vec2 TexCoord;\n"
"flat in uint cont;\n"
"layout (binding = 0) uniform sampler2DArray textureArray;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0 / cont, 1.0 / cont, 1.0 / cont, 1.0);\n"
"}\n\0";

#include "core/window.h"
#include "signal/signal_types.h"
#include "signal/signal_queue.h"
#include "signal/signal_layer.h"
#include "core/input.h"
#include "core/interrupts.h"
#include "util/platform.h"
#include "util/file_system.h"
#include "scene/static_model.h"
#include "gl/program.h"
#include "core/resource_manager.h"
#include "scene/model.h"

#include "test_instrumentor.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{

    gore::signal_layer layer;
    gore::window wnd(&layer);
    gore::signal_queue queue(&layer);
    gore::resource_manager rm(&queue);

    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    
    constexpr int how_many = 4;
    constexpr int vert_count = 4;
    constexpr int state = 1000;

    unsigned int vao, vbo, vbo2, ebo;
    glCreateVertexArrays(1, &vao);

    glEnableVertexArrayAttrib(vao, 0);
    glVertexArrayAttribBinding(vao, 0, 0);
    glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, 0);

    glEnableVertexArrayAttrib(vao, 1);
    glVertexArrayAttribBinding(vao, 1, 0);
    glVertexArrayAttribFormat(vao, 1, 2, GL_FLOAT, GL_FALSE, 4 * 3);

    glEnableVertexArrayAttrib(vao, 2);
    glVertexArrayAttribBinding(vao, 2, 1);
    glVertexArrayAttribIFormat(vao, 2, 1, GL_UNSIGNED_INT, 0);

    glCreateBuffers(1, &vbo);
    glNamedBufferStorage(vbo, 128 * 128 * 128 * how_many, nullptr, GL_DYNAMIC_STORAGE_BIT);
    glCreateBuffers(1, &vbo2);
    glNamedBufferStorage(vbo2, vert_count * sizeof(float) * 5 * how_many, nullptr, GL_DYNAMIC_STORAGE_BIT);
    glCreateBuffers(1, &ebo);
    glNamedBufferStorage(ebo, vert_count * sizeof(float) * 5 * how_many, nullptr, GL_DYNAMIC_STORAGE_BIT);

    int under = 0;
    struct vrtx
    {
        float pos[3];
        float tex[2];
        //unsigned int cnt;
    };
    for (unsigned int i = 0; i < how_many; i++)
    {
        if (how_many % 20 == 0)
            under++;
        vrtx vertices___[4] = {
            { 0.1f - (i * 0.2f),  0.1f - (i * 0.2f), 0.0f,0.0f,0.0f},  // top right
            { 0.1f - (i * 0.2f), -0.1f - (i * 0.2f), 0.0f,0.0f,0.0f},  // bottom right
            {-0.1f - (i * 0.2f), -0.1f - (i * 0.2f), 0.0f,0.0f,0.0f},  // bottom left
            {-0.1f - (i * 0.2f),  0.1f - (i * 0.2f), 0.0f,0.0f,0.0f}  // top left 
        };
        float vertices_[] = {
            1.00f - (i * 0.1f), 1.00f - (under * 0.1f), 0.0f, 1.0f, 1.0f,// top right
            0.90f - (i * 0.1f), 0.90f - (under * 0.1f), 0.0f, 1.0f, 0.0f,// bottom right
            1.00f - (i * 0.1f), 0.90f - (under * 0.1f), 0.0f, 0.0f, 1.0f,// top left                                         
        };
        float vertices[] = {
         0.1f - (i * 0.2f),  0.1f - (i * 0.2f), 0.0f,0.0f,0.0f,  // top right
         0.1f - (i * 0.2f), -0.1f - (i * 0.2f), 0.0f,0.0f,0.0f,  // bottom right
        -0.1f - (i * 0.2f), -0.1f - (i * 0.2f), 0.0f,0.0f,0.0f,  // bottom left
        -0.1f - (i * 0.2f),  0.1f - (i * 0.2f), 0.0f,0.0f,0.0f   // top left 
    };
    unsigned int indices[] = {  // note that we start from 0!
        0 + (i * 4), 1 + (i * 4), 3 + (i * 4),  // first Triangle
        1 + (i * 4), 2 + (i * 4), 3 + (i * 4)   // second Triangle
    };

        glNamedBufferSubData(vbo, i * sizeof(vertices___), sizeof(vertices___), vertices___);
        glNamedBufferSubData(ebo, i * sizeof(indices), sizeof(indices), indices);
    }
    unsigned int counter[] = {
        0, 1, 2, 3
    };
        glNamedBufferSubData(vbo2, 0 * sizeof(counter), sizeof(counter), counter);


    struct draw_command
    {
        unsigned int count_;
				unsigned int ins_count_;
				unsigned int first_index_;
				unsigned int base_vertex_;
				unsigned int base_ins_;
    };


    std::array<draw_command, how_many> arr;
    
    for (size_t i = 0; i < how_many; i++)
    {
        arr[i].count_ = 6;
        arr[i].ins_count_ = 1;
        arr[i].first_index_ = i * 6;
        arr[i].base_vertex_ = 0;
        arr[i].base_ins_ = i;
    }
    
   
    unsigned int dc;
    glGenBuffers(1, &dc);
    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, dc);
    glBufferData(GL_DRAW_INDIRECT_BUFFER, sizeof(draw_command) * how_many, arr.data(), GL_STATIC_DRAW);


    while (!wnd.should_close())
    {
        // input
        // -----

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(vao); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glVertexArrayBindingDivisor(vao, 1, 1);
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        glVertexArrayVertexBuffer(vao, 0, vbo, 0, 4 * 5);
        glVertexArrayVertexBuffer(vao, 1, vbo2, 0, 4);
        glVertexArrayElementBuffer(vao, ebo);
        glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, 0, how_many, 0);
        // glBindVertexArray(0); // no need to unbind it every time 
 
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        wnd.update();
		queue.handle_all();
		wnd.swap();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteProgram(shaderProgram);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}