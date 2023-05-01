add_subdirectory(imgui)
#add_subdirectory(tracy)

add_library(gore_engine STATIC
    core/game_state.cpp
    core/input.cpp
    #core/renderer.cpp
    core/timer.cpp
    core/window.cpp
    gl/buffer.cpp
    gl/constant_buffer.cpp
    gl/dynamic_buffer.cpp
    gl/globject.cpp
    gl/material.cpp
    gl/vertex_array.cpp 
    gl/program.cpp
    gl/shader.cpp
    gl/static_buffer.cpp
    gl/texture_pool.cpp
    memory/buffer_storage.cpp
    memory/program_storage.cpp
    memory/render_data_storage.cpp
    memory/renderable_storage.cpp
    memory/texture_storage.cpp
    physics/collision_body.cpp
    physics/shape_types.cpp
    physics/static_body.cpp
    scene/aabb.cpp
    scene/area.cpp
    scene/camera.cpp
    scene/constant_model.cpp
    scene/dynamic_model.cpp
    scene/light.cpp
    scene/mesh.cpp
    scene/model_instance.cpp
    scene/model.cpp
    scene/node.cpp
    scene/scene_node.cpp
    scene/scene_visual.cpp
    scene/spatial.cpp
    scene/static_model.cpp
    signal/signal_layer.cpp
    signal/signal_queue.cpp
    util/error_handler.cpp
    util/level_editor.cpp
    util/logger.cpp  
    util/platform.cpp
    util/util.cpp)
add_library(gore::engine ALIAS gore_engine)

target_include_directories(gore_engine 
    PRIVATE ${CMAKE_SOURCE_DIR}/include
    PRIVATE ${CMAKE_SOURCE_DIR}/mouthful_of_blood #this is bad
)