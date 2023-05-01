cmake_minimum_required(VERSION 3.22.1)

project(gore)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

find_package(OpenGL REQUIRED)
if (UNIX)
    find_package(X11 REQUIRED)
    find_package(Threads REQUIRED)
endif (UNIX)

add_compile_definitions(__GORE_DEBUG __GORE_EDITOR TRACY_ENABLE NOMINMAX)
add_compile_options(-Wint-to-pointer-cast -Wvolatile)
add_compile_options(-Wno-int-to-pointer-cast -Wno-volatile)

link_directories(${CMAKE_SOURCE_DIR}/lib)

add_subdirectory(src)
add_subdirectory(mouthful_of_blood)

add_executable(${PROJECT_NAME}
    level_creator.cpp 
    ${CMAKE_SOURCE_DIR}/src/glad/glad.c
)

target_include_directories(${PROJECT_NAME} 
    PRIVATE ${CMAKE_SOURCE_DIR}/include 
    PRIVATE ${CMAKE_SOURCE_DIR}/mouthful_of_blood
)
target_link_libraries(${PROJECT_NAME}
    mouthful::blood
    gore::engine 
    imgui::gui
    #tracy::profiler
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
