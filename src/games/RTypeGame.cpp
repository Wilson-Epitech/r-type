/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** RTypeGame
*/

#include "RTypeGame.hpp"

game::RTypeGame::RTypeGame(const std::string gameName) : AGame(gameName)
{
    initializeGame();
}

void game::RTypeGame::spawnEntities()
{
    _stringToEntity[BACKGROUND_NAME] =  _reg.spawnEntity();
    _stringToEntity[PLAYER_NAME] =  _reg.spawnEntity();
    _stringToEntity[FIRST_ENEMY_NAME] =  _reg.spawnEntity();
    _stringToEntity[SECOND_ENEMY_NAME] =  _reg.spawnEntity();
    _stringToEntity[THIRD_ENEMY_NAME] =  _reg.spawnEntity();
    _stringToEntity[SPAWN_ENEMY_NAME] =  _reg.spawnEntity();
    _stringToEntity[PLAYER_SHOOT] =  _reg.spawnEntity();
}

void game::RTypeGame::registerComponents()
{
    _reg.registerComponent<ecs::component::Position>();
    _reg.registerComponent<ecs::component::Velocity>();
    _reg.registerComponent<ecs::component::Drawable>();
    _reg.registerComponent<ecs::component::Controllable>();
    _reg.registerComponent<ecs::component::Type>();
    _reg.registerComponent<ecs::component::Life>();
    _reg.registerComponent<ecs::component::EnemySpawn>();
    _reg.registerComponent<ecs::component::SpriteInfos>();
}

void game::RTypeGame::addComponentsToEntities()
{
    ecs::SparseArray<ecs::component::Position> &positions = _reg.getComponents<ecs::component::Position>();
    ecs::SparseArray<ecs::component::Velocity> &velocities = _reg.getComponents<ecs::component::Velocity>();
    ecs::SparseArray<ecs::component::Drawable> &drawables = _reg.getComponents<ecs::component::Drawable>();
    ecs::SparseArray<ecs::component::Controllable> &controllables = _reg.getComponents<ecs::component::Controllable>();
    ecs::SparseArray<ecs::component::Type> &types = _reg.getComponents<ecs::component::Type>();
    ecs::SparseArray<ecs::component::Life> &lives = _reg.getComponents<ecs::component::Life>();
    ecs::SparseArray<ecs::component::EnemySpawn> &enemySpawns = _reg.getComponents<ecs::component::EnemySpawn>();
    ecs::SparseArray<ecs::component::SpriteInfos> &spriteInfos = _reg.getComponents<ecs::component::SpriteInfos>();

    positions.insertAt(_stringToEntity[PLAYER_NAME], ecs::component::Position{100, 100});
    velocities.insertAt(_stringToEntity[PLAYER_NAME], ecs::component::Velocity{0, 0, 5});
    controllables.insertAt(_stringToEntity[PLAYER_NAME], ecs::component::Controllable{});
    drawables.insertAt(_stringToEntity[PLAYER_NAME], ecs::component::Drawable{});
    spriteInfos.insertAt(_stringToEntity[PLAYER_NAME], ecs::component::SpriteInfos{PLAYER_PATH, PLAYER_SPRITE_NB_PATTERN,
        PLAYER_SPRITE_FIRST_PATTERN, PLAYER_SPRITE_SCALE_FACTOR});
    types.insertAt(_stringToEntity[PLAYER_NAME], ecs::component::Type{ecs::EntityType::PLAYER});
    lives.insertAt(_stringToEntity[PLAYER_NAME], ecs::component::Life{3});

    positions.insertAt(_stringToEntity[FIRST_ENEMY_NAME], ecs::component::Position{1500, 700});
    velocities.insertAt(_stringToEntity[FIRST_ENEMY_NAME], ecs::component::Velocity{-0.4f, 0});
    controllables.insertAt(_stringToEntity[FIRST_ENEMY_NAME], ecs::component::Controllable{false});
    drawables.insertAt(_stringToEntity[FIRST_ENEMY_NAME], ecs::component::Drawable{});
    spriteInfos.insertAt(_stringToEntity[FIRST_ENEMY_NAME], ecs::component::SpriteInfos{FIRST_ENEMY_PATH, FIRST_ENEMY_SPRITE_NB_PATTERN});
    types.insertAt(_stringToEntity[FIRST_ENEMY_NAME], ecs::component::Type{ecs::EntityType::ENEMY});
    lives.insertAt(_stringToEntity[FIRST_ENEMY_NAME], ecs::component::Life{1});

    positions.insertAt(_stringToEntity[SECOND_ENEMY_NAME], ecs::component::Position{1800, 300});
    velocities.insertAt(_stringToEntity[SECOND_ENEMY_NAME], ecs::component::Velocity{-0.3f, 0});
    drawables.insertAt(_stringToEntity[SECOND_ENEMY_NAME], ecs::component::Drawable{});
    spriteInfos.insertAt(_stringToEntity[SECOND_ENEMY_NAME], ecs::component::SpriteInfos{SECOND_ENEMY_PATH,
        SECOND_ENEMY_SPRITE_NB_PATTERN, SECOND_ENEMY_SPRITE_FIRST_PATTERN, SECOND_ENEMY_SPRITE_SCALE_FACTOR});
    types.insertAt(_stringToEntity[SECOND_ENEMY_NAME], ecs::component::Type{ecs::EntityType::ENEMY});
    lives.insertAt(_stringToEntity[SECOND_ENEMY_NAME], ecs::component::Life{1});

    positions.insertAt(_stringToEntity[THIRD_ENEMY_NAME], ecs::component::Position{1000, 300});
    velocities.insertAt(_stringToEntity[THIRD_ENEMY_NAME], ecs::component::Velocity{-0.3f, 0});
    drawables.insertAt(_stringToEntity[THIRD_ENEMY_NAME], ecs::component::Drawable{});
    spriteInfos.insertAt(_stringToEntity[THIRD_ENEMY_NAME], ecs::component::SpriteInfos{PLAYERSHOT_PATH, PLAYERSHOT_SPRITE_NB_PATTERN});
    types.insertAt(_stringToEntity[THIRD_ENEMY_NAME], ecs::component::Type{ecs::EntityType::ENEMY});
    lives.insertAt(_stringToEntity[THIRD_ENEMY_NAME], ecs::component::Life{1});

    positions.insertAt(_stringToEntity[BACKGROUND_NAME], ecs::component::Position{0, 0});
    velocities.insertAt(_stringToEntity[BACKGROUND_NAME], ecs::component::Velocity{-0.5f, 0.0});
    drawables.insertAt(_stringToEntity[BACKGROUND_NAME], ecs::component::Drawable{true});
    spriteInfos.insertAt(_stringToEntity[BACKGROUND_NAME], ecs::component::SpriteInfos{BACKGROUND_PATH, 1, 0, 0.625});

    spriteInfos.insertAt(_stringToEntity[SPAWN_ENEMY_NAME], ecs::component::SpriteInfos{FIRST_ENEMY_PATH, FIRST_ENEMY_SPRITE_NB_PATTERN, 0, ENEMY_SCALE_FACTOR});
    enemySpawns.insertAt(_stringToEntity[SPAWN_ENEMY_NAME], ecs::component::EnemySpawn{TIME_BEFORE_SPAWN_ENEMY, MAX_SPAWN_WIDTH, MIN_SPAWN_WIDTH, MAX_SPAWN_HEIGHT, MIN_SPAWN_HEIGHT});
    velocities.insertAt(_stringToEntity[SPAWN_ENEMY_NAME], ecs::component::Velocity{ENEMY_VX, ENEMY_VY});
    lives.insertAt(_stringToEntity[SPAWN_ENEMY_NAME], ecs::component::Life{1});

    velocities.insertAt(_stringToEntity[PLAYER_SHOOT], ecs::component::Velocity{10, 0});
    spriteInfos.insertAt(_stringToEntity[PLAYER_SHOOT], ecs::component::SpriteInfos{PLAYERSHOT_PATH, PLAYERSHOT_SPRITE_NB_PATTERN});
    types.insertAt(_stringToEntity[PLAYER_SHOOT], ecs::component::Type{ecs::EntityType::PLAYERSHOT});
}

void game::RTypeGame::addSystemsToRegister()
{
    _reg.addSystem(ecs::system::ControlSystem());
    _reg.addSystem(ecs::system::PositionSystem());
    _reg.addSystem(ecs::system::CollisionSystem());
    _reg.addSystem(ecs::system::LifeSystem());
    _reg.addSystem(ecs::system::FiringSystem());
    _reg.addSystem(ecs::system::EnemySpawnSystem());
    _reg.addSystem(ecs::system::DrawSystem());
    _reg.addSystem(ecs::system::SpriteAnimationSystem());
}

ecs::Registry &game::RTypeGame::getRegister()
{
    return _reg;
}

std::string &game::RTypeGame::getGameName()
{
    return _gameName;
}

game::GameStatus game::RTypeGame::gameState(ecs::Registry &reg)
{
    ecs::SparseArray<ecs::component::Life> &lives = reg.getComponents<ecs::component::Life>();
    ecs::SparseArray<ecs::component::Type> &types = reg.getComponents<ecs::component::Type>();
    entity_t entity = _stringToEntity[PLAYER_NAME];
    game::GameStatus status = game::GameStatus::WON;

    if (!lives[entity]) {
        status = game::GameStatus::LOOSE;
        return status;
    }
    for (std::size_t i = 0; i < lives.size() && i < types.size(); ++i) {
        if (lives[i] && types[i]) {
           if (types[i]->type == ecs::EntityType::ENEMY) {
                status = game::GameStatus::PLAYING;
                break;
           }
        }
    }
    return status;
}
