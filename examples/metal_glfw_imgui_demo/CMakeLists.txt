set(src ${EXAMPLE_SRC}/app.cpp)

# include dir and set header
include_directories(${METAL_HPP_DIR})

# add metal
set(headers ${METAL_HPP_DIR}/Metal.hpp)



# executable
add_executable(metal_example ${src} ${headers})

# link apple framework libraries
target_link_libraries(metal_example ${APPLE_FRAMEWORK_DEPS})
