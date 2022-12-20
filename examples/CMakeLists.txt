set(SHADER_RESOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders/example_shader.glsl
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders/mandelbrot)

add_subdirectory(metal_demo)
add_subdirectory(imgui_glfw_metal_demo)
