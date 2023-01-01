include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# FILE(GLOB FOUNDATION_HEADERS ${METAL_DIR}/Foundation/*.hpp)
# FILE(GLOB METAL_HEADERS ${METAL_DIR}/Metal/*.hpp)
# FILE(GLOB METALKIT_HEADERS ${METAL_DIR}/MetalKit/*.hpp)
# FILE(GLOB APPKIT_HEADERS ${METAL_DIR}/AppKit/*.hpp)
# FILE(GLOB QUARTZCORE_HEADERS ${METAL_DIR}/QuartzCore/*.hpp)

FILE(COPY ${SHADER_RESOURCES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/shaders)

# Add sources
set(src 
    ${CMAKE_CURRENT_SOURCE_DIR}/App.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Renderer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ExampleViewDelegate.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ExampleAppDelegate.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CoreTestViewDelegate.cpp
    )

# Add metal headers
FILE(GLOB EXAMPLE_HEDAERS ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)
set(headers
    ${EXAMPLE_HEDAERS}
    )

# Executable
add_compile_options(-fno-objc-arc)
add_executable(metal_example ${src} ${headers})
# Glfw compile definition
target_compile_definitions(metal_example PRIVATE GLFW_INCLUDE_NONE)
# Link 3rdparty libraries
target_link_libraries(metal_example ${APPLE_FRAMEWORK_DEPS} AnimationSystem metal)
