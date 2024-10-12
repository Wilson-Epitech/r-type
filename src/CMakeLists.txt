set(SRC
    gameEngine/GameEngine.cpp
    games/RtypeGameClient.cpp
    games/RtypeGameServer.cpp
    ecs/components/Components.cpp
    ecs/systems/CollisionSystem.cpp
    ecs/systems/ControlSystem.cpp
    ecs/systems/DrawSystem.cpp
    ecs/systems/EnemyFiringSystem.cpp
    ecs/systems/EnemySpawnSystem.cpp
    ecs/systems/FiringSystem.cpp
    ecs/systems/KillOffscreenEntitiesSystem.cpp
    ecs/systems/LifeSystem.cpp
    ecs/systems/PlayerSpawnSystem.cpp
    ecs/systems/PositionSystem.cpp
    ecs/systems/SoundSystem.cpp
    ecs/systems/SpriteAnimationSystem.cpp
    ecs/systems/MusicSystem.cpp
    ../lib/Lib.cpp
)

add_library(rtype_lib ${SRC})

target_link_libraries(rtype_lib PUBLIC raylib)

target_include_directories(rtype_lib PUBLIC
    ../include
    ecs
    ecs/components
    ecs/systems
    gameEngine
    games
    ../lib
)
