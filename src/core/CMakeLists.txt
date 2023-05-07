add_library(gcore STATIC
    #SOURCE

    window/glfw_window.cpp

    #HEADER

    ${CMAKE_SOURCE_DIR}/include/core/window/generic_window.h
    ${CMAKE_SOURCE_DIR}/include/core/window/glfw_window.h
)
add_library(gore::gcore ALIAS gcore)

target_include_directories(gcore
    PRIVATE ${CMAKE_SOURCE_DIR}/include/core
)