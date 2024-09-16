cmake_minimum_required(VERSION 3.5.0)
project(r-type)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_options(-Wall -Wextra -Werror)

include_directories(include)

set(SRC
    ../src/main.cpp
)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_BINARY_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake"
    CACHE STRING "Vcpkg toolchain file")

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build/cmakefiles)

add_executable(r-type_server ${SRC})
