# TODO: add imgui, glfw and metal headers
cmake_minimum_required(VERSION 3.20)

project(AnimationSystem LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

find_library(APPLICATIONSERVICES_LIBRARY ApplicationServices REQURED)
find_library(METAL_LIBRARY Metal REQURED)
find_library(OPENGL_LIBRARY OpenGL REQURED)
find_library(FOUNDATION_LIBRARY Foundation REQURED)
find_library(QUARTZ_LIBRARY Quartz REQURED)
find_library(GAME_CONTROLLER_LIBRARY GameController REQURED)
find_library(CORE_GRAPHICS_LIBGRARY CoreGraphics REQURED)
find_library(METAL_KIT_LIB MetalKit REQURED)

set(APPLE_FRAMEWORK_DEPS 
    "-framework Cocoa" 
    "-framework Metal" 
    "-framework MetalKit"
    "-framework QuartzCore" 
    "-framework GameController" 
    "-framework Foundation"
    "-framework CoreGraphics")

set(DEPENDENCIES_SRC_DIR ${PROJECT_SOURCE_DIR}/3rdparty)
set(DEPENDENCIES_BIN_DIR ${PROJECT_BINARY_DIR}/3rdparty)

set(METAL_DIR ${PROJECT_SOURCE_DIR}/3rdparty/metal)
set(METAL_INCLUDE_DIR
    ${METAL_DIR}/AppKit
    ${METAL_DIR}/MetalKit
    ${METAL_DIR}/Metal
    ${METAL_DIR}/Foundation
    ${METAL_DIR}/QuartzCore
    )

add_subdirectory(3rdparty)
add_subdirectory(examples)
