cmake_minimum_required(VERSION 3.14)
project(r-type)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(UNIX)
    add_compile_options(-Wall -Wextra)
elseif(WIN32)
    add_definitions(-D_WIN32_WINNT=0x0A00)
endif()

set(VCPKG_ROOT "${CMAKE_SOURCE_DIR}/build/vcpkg")

if(NOT EXISTS "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
    message(STATUS "Downloading vcpkg...")
    execute_process(COMMAND git clone https://github.com/microsoft/vcpkg.git ${VCPKG_ROOT})
    if(UNIX)
        execute_process(COMMAND ${VCPKG_ROOT}/bootstrap-vcpkg.sh)
    elseif(WIN32)
        execute_process(COMMAND ${VCPKG_ROOT}/bootstrap-vcpkg.bat)
        set(VCPKG_TARGET_TRIPLET "x64-mingw-dynamic" CACHE STRING "")
    endif()
    execute_process(COMMAND ${VCPKG_ROOT}/vcpkg install raylib asio glfw3 --triplet ${VCPKG_TARGET_TRIPLET})
endif()

set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")

if(UNIX)
    find_path(RAYLIB_INCLUDE_DIR raylib.h PATHS "${VCPKG_ROOT}/installed/${VCPKG_TARGET_TRIPLET}/include")
    find_library(RAYLIB_LIBRARY NAMES raylib PATHS "${VCPKG_ROOT}/installed/${VCPKG_TARGET_TRIPLET}/lib")

    find_path(ASIO_INCLUDE_DIR asio.hpp PATHS "${VCPKG_ROOT}/installed/${VCPKG_TARGET_TRIPLET}/include")

    if(NOT RAYLIB_INCLUDE_DIR OR NOT RAYLIB_LIBRARY)
        message(FATAL_ERROR "raylib not found")
    endif()

    if(NOT ASIO_INCLUDE_DIR)
        message(FATAL_ERROR "asio not found")
    endif()

    add_library(raylib INTERFACE)
    target_include_directories(raylib INTERFACE ${RAYLIB_INCLUDE_DIR})
    target_link_libraries(raylib INTERFACE ${RAYLIB_LIBRARY})

    add_library(asio INTERFACE)
    target_include_directories(asio INTERFACE ${ASIO_INCLUDE_DIR})
elseif(WIN32)
    include_directories(${VCPKG_ROOT}/installed/${VCPKG_TARGET_TRIPLET}/include)
    link_directories(${VCPKG_ROOT}/installed/${VCPKG_TARGET_TRIPLET}/lib)

    add_library(raylib INTERFACE)
    target_include_directories(raylib INTERFACE ${VCPKG_ROOT}/installed/${VCPKG_TARGET_TRIPLET}/include)
    target_link_libraries(raylib INTERFACE raylib)

    add_library(asio INTERFACE)
    target_include_directories(asio INTERFACE ${VCPKG_ROOT}/installed/${VCPKG_TARGET_TRIPLET}/include)
endif()

add_subdirectory(src)
add_subdirectory(src/network/client)
add_subdirectory(src/network/server)