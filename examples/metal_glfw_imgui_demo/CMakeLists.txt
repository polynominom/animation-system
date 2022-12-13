set(src ${EXAMPLE_SRC}/app.cpp)

# include dir and set header
include_directories(${METAL_HPP_DIR})

# add metal
set(headers ${METAL_HPP_DIR}/Metal.hpp)

# executable
add_executable(metal_example ${src} ${headers})

# glfw compile definition
target_compile_definitions(metal_example PRIVATE GLFW_INCLUDE_NONE)
# link apple framework libraries
target_link_libraries(metal_example ${APPLE_FRAMEWORK_DEPS} glfw imgui)
