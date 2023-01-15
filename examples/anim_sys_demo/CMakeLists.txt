include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${COMMON_DEMO_FOLDER})

FILE(COPY ${SHADER_RESOURCES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

set(CHARACTER_MODEL_RES ${CMAKE_CURRENT_SOURCE_DIR}/character_model)

FILE(COPY ${CHARACTER_MODEL_RES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# Add sources
set(src 
    ${COMMON_DEMO_SRC}
    ${CMAKE_CURRENT_SOURCE_DIR}/CoreTestViewDelegate.cpp
    )

# Add metal headers
FILE(GLOB EXAMPLE_HEDAERS ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)
set(headers
    ${COMMON_DEMO_HDRS}
    ${EXAMPLE_HEDAERS}
    )

# Executable
add_compile_options(-fno-objc-arc)
add_executable(anim_sys_example ${src} ${headers})

target_compile_definitions(anim_sys_example PUBLIC -DCHARACTER_MODEL_FOLDER="${CHARACTER_MODEL_RES}")
target_compile_definitions(anim_sys_example PUBLIC -DSHADER_FOLDER="${SHADER_RESOURCES}")

target_compile_definitions(anim_sys_example PUBLIC COREDEMO)
# Glfw compile definition
target_compile_definitions(anim_sys_example PRIVATE GLFW_INCLUDE_NONE)
# Link 3rdparty libraries
target_link_libraries(anim_sys_example ${APPLE_FRAMEWORK_DEPS} AnimationSystem metal)
