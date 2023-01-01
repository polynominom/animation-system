cmake_minimum_required(VERSION 3.20)

# GLFW dependency
# Set some options before adding GLFW in to your project
# to prevent cluttered projects that are not needed.
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL OFF)
set(GLFW_VULKAN_STATIC OFF)
add_subdirectory(glfw)
set_target_properties(glfw PROPERTIES FOLDER "Dependencies")

# HANDLE IMGUI
project(IMGUI LANGUAGES CXX)

include_directories(${DEPENDENCIES_SRC_DIR}/glfw/include)

set(IMGUI_SRCS 
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_tables.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_draw.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_widgets.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_demo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_opengl3.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_glfw.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_metal.mm
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_osx.mm
    )

set(IMGUI_HDRS
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imconfig.h
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui.h
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_internal.h
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imstb_rectpack.h
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imstb_textedit.h
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imstb_truetype.h)

add_library(imgui STATIC ${IMGUI_SRCS} ${IMGUI_HDRS})
target_include_directories(imgui PUBLIC ${IMGUI_HDRS})

target_compile_definitions(imgui PRIVATE IMGUI_IMPL_OPENGL_LOADER_GLAD)
target_link_libraries(imgui PRIVATE glfw ${OPENGL_LIBRARY})

set_target_properties(imgui PROPERTIES FOLDER "Dependencies")


add_library(metal INTERFACE)
include_directories(${METAL_DIR})
target_include_directories(metal INTERFACE ${METAL_DIR})

# Link 3rdparty libraries
# target_link_libraries(metal ${APPLE_FRAMEWORK_DEPS} AnimationSystem)
