set(SHADER_RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/shaders)
FILE(GLOB SHADER_RESOURCES_FILES ${CMAKE_CURRENT_SOURCE_DIR}/shader/*)
FILE(GLOB COMMON_DEMO_SRC ${CMAKE_CURRENT_SOURCE_DIR}/_commons/*.cpp)
FILE(GLOB COMMON_DEMO_HDRS ${CMAKE_CURRENT_SOURCE_DIR}/_commons/*.hpp)
set(COMMON_DEMO_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/_commons)

add_subdirectory(anim_sys_demo)
add_subdirectory(metal_demo)
add_subdirectory(imgui_glfw_metal_demo)
