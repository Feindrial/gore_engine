#ifndef GORE_TEST_INSTRUMENTOR_H
#define GORE_TEST_INSTRUMENTOR_H

#include "GLFW/glfw3.h"

#include <iostream>

namespace gore
{
    class test_instrumentor
    {
    public:
        static test_instrumentor& get() { static test_instrumentor ti; return ti; }
        inline void start() { start_ = glfwGetTime(); }
        inline double stop() {  return glfwGetTime() - start_; }
    private:
        double start_{0};
    };
}

#endif