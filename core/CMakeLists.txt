cmake_minimum_required(VERSION 3.20)
project(AnimationSystemCore VERSION 0.0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB fs_src ${CMAKE_CURRENT_SOURCE_DIR}/fs/*.cpp)
set(fs_hdrs 
        ${CMAKE_CURRENT_SOURCE_DIR}/fs/FS.h
        ${CMAKE_CURRENT_SOURCE_DIR}/fs/Reader.hpp
)

set(hdrs 
    ${fs_hdrs}
    ${CMAKE_CURRENT_SOURCE_DIR}/AnimationSystem.h)

add_library(${PROJECT_NAME} STATIC ${fs_src} ${hdrs})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# target_sources(${PROJECT_NAME} PRIVATE ${fs_src})

# source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${hdrs})
# source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${fs_src})
