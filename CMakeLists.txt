cmake_minimum_required(VERSION 3.25.0)

project(gore)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

find_package(OpenGL REQUIRED)
if (UNIX)
    find_package(X11 REQUIRED)
    find_package(Threads REQUIRED)
endif (UNIX)

add_compile_definitions(__GORE_DEBUG __GORE_EDITOR NOMINMAX)

link_directories(${CMAKE_SOURCE_DIR}/lib)

add_subdirectory(src)

add_executable(${PROJECT_NAME}
    level_creator.cpp 
    ${CMAKE_SOURCE_DIR}/src/glad/glad.c
)

target_include_directories(${PROJECT_NAME} 
    PUBLIC ${CMAKE_SOURCE_DIR}/include 
)
target_link_libraries(${PROJECT_NAME}
    gore::engine 
    imgui::gui
    glfw3
    ${OPENGL_LIBRARIES} 
    assimp
)

if (UNIX)
    target_link_libraries(${PROJECT_NAME}
        Threads::Threads
        ${X11_LIBRARIES}
        ${CMAKE_DL_LIBS}
    )
endif (UNIX)
