set(SRC
    games/RtypeGameClient.cpp
    games/RtypeGameServer.cpp
    ecs/components/Components.cpp
    ecs/systems/CollisionSystem.cpp
    ecs/systems/ControlSystem.cpp
    ecs/systems/DrawSpriteSystem.cpp
    ecs/systems/DrawLifeBarSystem.cpp
    ecs/systems/DrawTextSystem.cpp
    ecs/systems/EnemyFiringSystem.cpp
    ecs/systems/BossFiringSystem.cpp
    ecs/systems/EnemySpawnSystem.cpp
    ecs/systems/BossSpawnSystem.cpp
    ecs/systems/FiringSystem.cpp
    ecs/systems/KillOffscreenEntitiesSystem.cpp
    ecs/systems/LifeSystem.cpp
    ecs/systems/PlayerSpawnSystem.cpp
    ecs/systems/PositionSystem.cpp
    ecs/systems/SoundSystem.cpp
    ecs/systems/SpriteAnimationSystem.cpp
    ecs/systems/MusicSystem.cpp
    ecs/systems/ScoreSystem.cpp
    ecs/systems/PowerupSystem.cpp
    ecs/systems/ResetForNextLevel.cpp
    ecs/systems/WeaponSystem.cpp
    runGame/RunGame.cpp
    network/handleData/HandleData.cpp
    ../lib/Lib.cpp
)

add_library(rtype_lib ${SRC})

target_link_libraries(rtype_lib PRIVATE raylib asio glfw)

target_include_directories(rtype_lib PUBLIC
    ../include
    ecs
    ecs/components
    ecs/systems
    gameEngine
    games
    runGame
    network
    network/handleData
    network/server
    network/client
    ../lib
)
