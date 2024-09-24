cmake_minimum_required(VERSION 3.14)
project(r-type)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra)

set(CMAKE_TOOLCHAIN_FILE $ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake CACHE STRING "")

find_package(raylib CONFIG REQUIRED)

include_directories(
    ../include
    ../src/ecs
    ../src/ecs/components
    ../src/ecs/systems
    ../src/games
    ../src/gameEngine
    ../lib
)

file(GLOB_RECURSE SRC
    ../src/main.cpp
    ../src/gameEngine/GameEngine.cpp
    ../src/games/RTypeGame.cpp
    ../src/ecs/systems/CollisionSystem.cpp
    ../src/ecs/systems/ControlSystem.cpp
    ../src/ecs/systems/DrawSystem.cpp
    ../src/ecs/systems/FiringSystem.cpp
    ../src/ecs/systems/LifeSystem.cpp
    ../src/ecs/systems/EnemySpawnSystem.cpp
    ../src/ecs/systems/PositionSystem.cpp
    ../src/ecs/systems/SpriteAnimationSystem.cpp
    ../src/ecs/systems/CollisionSystem.cpp
    ../src/ecs/systems/FiringSystem.cpp
    ../src/ecs/systems/SpriteAnimationSystem.cpp
    ../src/ecs/systems/ControlSystem.cpp
    ../src/ecs/systems/LifeSystem.cpp
    ../src/ecs/systems/DrawSystem.cpp
    ../src/ecs/systems/PositionSystem.cpp
    ../lib/Lib.cpp
)

add_executable(r-type ${SRC})

# macOS #
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    target_link_libraries(r-type
        PRIVATE raylib
        "-framework OpenGL"
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo"
        "-framework GLUT"
    )

# Linux #
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_link_libraries(r-type
        raylib
        GL
        m
        pthread
        dl
        rt
        X11
    )
else()
    message(FATAL_ERROR "Unsupported OS")
endif()