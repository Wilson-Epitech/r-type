cmake_minimum_required(VERSION 3.14)
project(r-type)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(VCPKG_ROOT "${CMAKE_SOURCE_DIR}/build/vcpkg")

if(UNIX)
    add_compile_options(-Wall -Wextra)
elseif(WIN32)
    add_definitions(-D_WIN32_WINNT=0x0A00)
endif()

option(BUILD_SHARED_LIBS ON)

include_directories("${VCPKG_ROOT}/installed/${VCPKG_TARGET_TRIPLET}/include")
include_directories("${VCPKG_ROOT}/installed/${VCPKG_TARGET_TRIPLET}/include/GLFW")

# message(STATUS "INCLUDE DIRECTORY: ${VCPKG_ROOT}/installed/${VCPKG_TARGET_TRIPLET}/include/asio")

find_package(raylib CONFIG REQUIRED)
find_package(asio CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

add_subdirectory(src)
add_subdirectory(src/network/client)
add_subdirectory(src/network/server)
