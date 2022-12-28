cmake_minimum_required(VERSION 3.20)
project(AnimationSystem VERSION 0.0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR} 
    ${CMAKE_CURRENT_SOURCE_DIR}/Core
    ${CMAKE_CURRENT_SOURCE_DIR}/Base
    ${CMAKE_CURRENT_SOURCE_DIR}/Animation)

# Get CORE files - TODO: convert this process to a custom function(s)?
file(GLOB core_src ${CMAKE_CURRENT_SOURCE_DIR}/Core/*.cpp)
file(GLOB core_fs_src ${CMAKE_CURRENT_SOURCE_DIR}/Core/Fs/*.cpp)
file(GLOB core_task_src ${CMAKE_CURRENT_SOURCE_DIR}/Core/Task/*.cpp)
set(core_src 
        ${core_src}
        ${core_fs_src}
        ${core_task_src}
)

file(GLOB core_hdrs ${CMAKE_CURRENT_SOURCE_DIR}/Core/*.hpp)
file(GLOB core_fs_hdrs ${CMAKE_CURRENT_SOURCE_DIR}/Core/Fs/*.hpp)
file(GLOB core_task_hdrs ${CMAKE_CURRENT_SOURCE_DIR}/Core/Task/*.hpp)
set(core_hdrs 
        ${core_hdrs}
        ${core_fs_hdrs}
        ${core_task_hdrs}
)

# Get Base files
file(GLOB base_src ${CMAKE_CURRENT_SOURCE_DIR}/Base/*.cpp)
file(GLOB base_hdrs ${CMAKE_CURRENT_SOURCE_DIR}/Base/*.hpp)

# Get Animation Files
file(GLOB anim_src ${CMAKE_CURRENT_SOURCE_DIR}/Animation/*.cpp)
file(GLOB anim_hdrs ${CMAKE_CURRENT_SOURCE_DIR}/Animation/*.hpp)

# Set headers and soruces variables
set(hdrs 
    ${core_hdrs}
    ${base_hdrs}
    ${anim_hdrs}
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Core.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Base/Base.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Animation/Animation.h
)

set(src 
    ${core_src}
    ${base_src}
    ${anim_src}
)

add_library(${PROJECT_NAME} STATIC ${src} ${hdrs})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
