include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${COMMON_DEMO_FOLDER})

FILE(COPY ${SHADER_RESOURCES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
FILE(COPY ${MESH_RESOURCES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
set_source_files_properties(${SHADER_RESOURCES_FILES} PROPERTIES FOLDER "Dependencies")
set_source_files_properties(${MESH_RESOURCES_FILES} PROPERTIES FOLDER "Dependencies")

# Add sources
set(src 
    ${COMMON_DEMO_SRC}
    ${CMAKE_CURRENT_SOURCE_DIR}/Renderer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ExampleViewDelegate.cpp
    )

# Add metal headers
FILE(GLOB EXAMPLE_HEDAERS ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)
set(headers
    ${COMMON_DEMO_HDRS}
    ${EXAMPLE_HEDAERS}
    )

# Executable
add_compile_options(-fno-objc-arc)
add_executable(metal_example ${src} ${headers})
target_compile_definitions(metal_example PUBLIC METALDEMO)
# Glfw compile definition
target_compile_definitions(metal_example PRIVATE GLFW_INCLUDE_NONE)
# Link 3rdparty libraries
target_link_libraries(metal_example ${APPLE_FRAMEWORK_DEPS} AnimationSystem metal)
