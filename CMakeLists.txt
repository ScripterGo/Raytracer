
cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_COMPILER /usr/bin/clang++)

project(ray_tracing
    VERSION 1.0
    LANGUAGES CXX
)

add_executable(ray_tracing
    main.cpp
    src/surface.cpp
    src/camera.cpp
    src/scene.cpp
    src/sphere.cpp
    src/plane.cpp
    src/point_light.cpp
    headers/util.hpp
    headers/math_lib.hpp
)

configure_file(ray_tracing_config.h.in ray_tracing_config.h)

#target_compile_options(ray_tracing PRIVATE -Isrc/ -Wall -Wextra -g)
target_compile_options(ray_tracing PRIVATE -Isrc/ -Wall -Wextra)

