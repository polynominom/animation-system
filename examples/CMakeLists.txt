set(SHADER_RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/shaders)
FILE(GLOB SHADER_RESOURCES_FILES ${CMAKE_CURRENT_SOURCE_DIR}/shader/*)

add_subdirectory(metal_demo)
add_subdirectory(imgui_glfw_metal_demo)
