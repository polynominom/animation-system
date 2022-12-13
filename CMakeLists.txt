# TODO: add imgui, glfw and metal headers
cmake_minimum_required(VERSION 3.20)

project(AnimationSystem LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

find_library(APPLICATIONSERVICES_LIBRARY ApplicationServices)
find_library(METAL_LIBRARY Metal)
find_library(OPENGL_LIBRARY OpenGL)
find_library(FOUNDATION_LIBRARY Foundation)
find_library(QUARTZ_LIBRARY Quartz)

set(APPLE_FRAMEWORK_DEPS 
    "-framework Cocoa" 
    "-framework Metal" 
    "-framework QuartzCore" 
    "-framework Foundation")

set(METAL_HPP_DIR ${PROJECT_SOURCE_DIR}/3rdparty/metal)
set(GLFW_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/glfw/include)

set(EXAMPLE_SRC ${PROJECT_SOURCE_DIR}/examples/metal_glfw_imgui_demo)

add_subdirectory(examples)
