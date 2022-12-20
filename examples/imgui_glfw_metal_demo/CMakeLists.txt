include_directories(${METAL_DIR} 
                    ${CMAKE_CURRENT_SOURCE_DIR}
                    ${DEPENDENCIES_SRC_DIR}/imgui)

FILE(GLOB FOUNDATION_HEADERS ${METAL_DIR}/Foundation/*.hpp)
FILE(GLOB METAL_HEADERS ${METAL_DIR}/Metal/*.hpp)
FILE(GLOB METALKIT_HEADERS ${METAL_DIR}/MetalKit/*.hpp)
FILE(GLOB APPKIT_HEADERS ${METAL_DIR}/AppKit/*.hpp)
FILE(GLOB QUARTZCORE_HEADERS ${METAL_DIR}/QuartzCore/*.hpp)

FILE(COPY ${SHADER_RESOURCES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/shaders)

# Add sources
FILE(GLOB CXX_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
FILE(GLOB MM_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.mm)
set(src ${SOURCES} ${MM_SOURCES})

# Add metal headers
FILE(GLOB EXAMPLE_HEDAERS ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)
set(headers
    ${EXAMPLE_HEDAERS}
    ${FOUNDATION_HEADERS}
    ${METALKIT_HEADERS}
    ${METAL_HEADERS}
    ${APPKIT_HEADERS}
    ${QUARTZCORE_HEADERS}
    )

add_executable(ImguiGLFWMetalDemo ${src} ${hdrs})

# Glfw compile definition
target_compile_definitions(ImguiGLFWMetalDemo PRIVATE GLFW_INCLUDE_NONE)
# Link 3rdparty libraries
target_link_libraries(ImguiGLFWMetalDemo ${APPLE_FRAMEWORK_DEPS} AnimationSystemCore imgui glfw)
