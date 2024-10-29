/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** RtypeGameServer
*/

#include "RtypeGameServer.hpp"

#include <iostream>

game::RtypeGameServer::RtypeGameServer(const std::string difficultyMode, const std::string gameName) : AGame(gameName) {
    initStringToDifficultyMode();
    setGameDifficulty(difficultyMode);
    initializeGame();
}

void game::RtypeGameServer::spawnEntities() {
    _stringToEntity[BACKGROUND_NAME] = _reg.spawnEntity();
    _stringToEntity[PLAYER_NAME] = _reg.spawnEntity();
    _stringToEntity[PLAYER_SHOT_NAME] = _reg.spawnEntity();
    _stringToEntity[SPAWN_ENEMY_NAME] = _reg.spawnEntity();
    _stringToEntity[ENEMY_SHOT_NAME] = _reg.spawnEntity();
    _stringToEntity[SPAWN_STRONGER_ENEMY_NAME] = _reg.spawnEntity();
    _stringToEntity[MUSIC_NAME] = _reg.spawnEntity();
    _stringToEntity[TEAM_SCORE_NAME] = _reg.spawnEntity();
    _stringToEntity[TITLE_NAME] = _reg.spawnEntity();
    _stringToEntity[POWERUP_NAME] = _reg.spawnEntity();
}

void game::RtypeGameServer::registerComponents() {
    _reg.registerComponent<ecs::component::Position>();
    _reg.registerComponent<ecs::component::Velocity>();
    _reg.registerComponent<ecs::component::Shooting>();
    _reg.registerComponent<ecs::component::Type>();
    _reg.registerComponent<ecs::component::SpriteInfos>();
    _reg.registerComponent<ecs::component::EnemySpawn>();
    _reg.registerComponent<ecs::component::Life>();
    _reg.registerComponent<ecs::component::SoundEffectInfos>();
    _reg.registerComponent<ecs::component::ShootingRate>();
    _reg.registerComponent<ecs::component::BackgroundMusicInfos>();
    _reg.registerComponent<ecs::component::Score>();
    _reg.registerComponent<ecs::component::ShootPlayerIndex>();
    _reg.registerComponent<ecs::component::TextInfos>();
    _reg.registerComponent<ecs::component::Powerup>();
    _reg.registerComponent<ecs::component::ActivePowerup>();
    _reg.registerComponent<ecs::component::MovementPattern>();
}

void game::RtypeGameServer::addComponentsToEntities() {
    ecs::SparseArray<ecs::component::Position> &positions = _reg.getComponents<ecs::component::Position>();
    ecs::SparseArray<ecs::component::Velocity> &velocities = _reg.getComponents<ecs::component::Velocity>();
    ecs::SparseArray<ecs::component::Shooting> &shootings = _reg.getComponents<ecs::component::Shooting>();
    ecs::SparseArray<ecs::component::Type> &types = _reg.getComponents<ecs::component::Type>();
    ecs::SparseArray<ecs::component::Life> &lives = _reg.getComponents<ecs::component::Life>();
    ecs::SparseArray<ecs::component::EnemySpawn> &enemySpawns = _reg.getComponents<ecs::component::EnemySpawn>();
    ecs::SparseArray<ecs::component::SpriteInfos> &spriteInfos = _reg.getComponents<ecs::component::SpriteInfos>();
    ecs::SparseArray<ecs::component::SoundEffectInfos> &soundEffectInfos = _reg.getComponents<ecs::component::SoundEffectInfos>();
    ecs::SparseArray<ecs::component::ShootingRate> &shootingRate = _reg.getComponents<ecs::component::ShootingRate>();
    ecs::SparseArray<ecs::component::BackgroundMusicInfos> &backgroundMusic = _reg.getComponents<ecs::component::BackgroundMusicInfos>();
    ecs::SparseArray<ecs::component::Score> &score = _reg.getComponents<ecs::component::Score>();
    ecs::SparseArray<ecs::component::TextInfos> &textInfos = _reg.getComponents<ecs::component::TextInfos>();
    ecs::SparseArray<ecs::component::Powerup> &powerups = _reg.getComponents<ecs::component::Powerup>();
    ecs::SparseArray<ecs::component::ActivePowerup> &activePowerups = _reg.getComponents<ecs::component::ActivePowerup>();
    ecs::SparseArray<ecs::component::MovementPattern> &movementPattern = _reg.getComponents<ecs::component::MovementPattern>();

    const float timeBeforeSpawnEnemy = (TIME_BEFORE_SPAWN_ENEMY + static_cast<int>(_mode)) / static_cast<int>(_mode);
    const float timeBeforeSpawnStrongerEnemy = (TIME_BEFORE_SPAWN_ENEMY + static_cast<int>(_mode)) / static_cast<int>(_mode);
    const std::size_t enemyWaveSize = ENEMY_WAVE_SIZE + (ENEMY_WAVE_SIZE_DIFFICULTY_FACTOR * static_cast<int>(_mode - 1));
    const std::size_t strongerEnemyWaveSize = STRONGER_ENEMY_WAVE_SIZE * (static_cast<int>(_mode - 1));

    positions.insertAt(_stringToEntity[BACKGROUND_NAME], ecs::component::Position{0, 0});
    velocities.insertAt(_stringToEntity[BACKGROUND_NAME], ecs::component::Velocity{-0.5, 0.0});
    spriteInfos.insertAt(_stringToEntity[BACKGROUND_NAME], ecs::component::SpriteInfos{BACKGROUND_PATH, 1, 0, 0.625});
    types.insertAt(_stringToEntity[BACKGROUND_NAME], ecs::component::Type{ecs::EntityType::BACKGROUND});

    velocities.insertAt(_stringToEntity[PLAYER_NAME], ecs::component::Velocity{0, 0, 8});
    spriteInfos.insertAt(
        _stringToEntity[PLAYER_NAME],
        ecs::component::SpriteInfos{PLAYER_PATH, PLAYER_SPRITE_NB_PATTERN, PLAYER_SPRITE_FIRST_PATTERN, PLAYER_SPRITE_SCALE_FACTOR});
    shootings.insertAt(_stringToEntity[PLAYER_NAME], ecs::component::Shooting{false});
    types.insertAt(_stringToEntity[PLAYER_NAME], ecs::component::Type{ecs::EntityType::PLAYER_TEMPLATE});
    lives.insertAt(_stringToEntity[PLAYER_NAME], ecs::component::Life{5, true});
    soundEffectInfos.insertAt(_stringToEntity[PLAYER_NAME], ecs::component::SoundEffectInfos{EXPLOSION_SOUND_PATH});
    shootingRate.insertAt(_stringToEntity[PLAYER_NAME], ecs::component::ShootingRate{0.3});
    score.insertAt(_stringToEntity[PLAYER_NAME], ecs::component::Score{100});
    textInfos.insertAt(_stringToEntity[PLAYER_NAME], ecs::component::TextInfos{"Score:", 50, 50, 20, FONT_PATH, ORANGE_COLOR});
    activePowerups.insertAt(_stringToEntity[PLAYER_NAME], ecs::component::ActivePowerup{});

    velocities.insertAt(_stringToEntity[PLAYER_SHOT_NAME], ecs::component::Velocity{10, 0});
    spriteInfos.insertAt(_stringToEntity[PLAYER_SHOT_NAME], ecs::component::SpriteInfos{PLAYERSHOT_PATH, PLAYERSHOT_SPRITE_NB_PATTERN});
    types.insertAt(_stringToEntity[PLAYER_SHOT_NAME], ecs::component::Type{ecs::EntityType::PLAYERSHOT_TEMPLATE});
    soundEffectInfos.insertAt(_stringToEntity[PLAYER_SHOT_NAME], ecs::component::SoundEffectInfos{SHOOT_SOUND_PATH});

    spriteInfos.insertAt(_stringToEntity[SPAWN_ENEMY_NAME],
                         ecs::component::SpriteInfos{ENEMY_PATH, ENEMY_SPRITE_NB_PATTERN, 0, ENEMY_SCALE_FACTOR});
    enemySpawns.insertAt(
        _stringToEntity[SPAWN_ENEMY_NAME],
        ecs::component::EnemySpawn{timeBeforeSpawnEnemy, enemyWaveSize, MAX_SPAWN_WIDTH, MIN_SPAWN_WIDTH, MAX_SPAWN_HEIGHT, MIN_SPAWN_HEIGHT});
    velocities.insertAt(_stringToEntity[SPAWN_ENEMY_NAME], ecs::component::Velocity{ENEMY_VX, ENEMY_VY});
    lives.insertAt(_stringToEntity[SPAWN_ENEMY_NAME], ecs::component::Life{ENEMY_NB_LIVES, false, true});
    shootingRate.insertAt(_stringToEntity[SPAWN_ENEMY_NAME], ecs::component::ShootingRate{ENEMY_SHOOTING_RATE});
    soundEffectInfos.insertAt(_stringToEntity[SPAWN_ENEMY_NAME], ecs::component::SoundEffectInfos{EXPLOSION_SOUND_PATH});
    movementPattern.insertAt(_stringToEntity[SPAWN_ENEMY_NAME], ecs::component::MovementPattern{ENEMY_PATTERN_AMPLITUDE, true, ecs::MovementType::SNAKE});

    velocities.insertAt(_stringToEntity[ENEMY_SHOT_NAME], ecs::component::Velocity{STRONGER_ENEMY_VX, STRONGER_ENEMY_VY});
    spriteInfos.insertAt(_stringToEntity[ENEMY_SHOT_NAME],
                         ecs::component::SpriteInfos{ENEMYSHOT_PATH, ENEMYSHOT_SPRITE_NB_PATTERN, 0, ENEMYSHOT_SCALE_FACTOR});
    types.insertAt(_stringToEntity[ENEMY_SHOT_NAME], ecs::component::Type{ecs::EntityType::ENEMYSHOT_TEMPLATE});
    soundEffectInfos.insertAt(_stringToEntity[ENEMY_SHOT_NAME], ecs::component::SoundEffectInfos{SHOOT_SOUND_PATH});

    spriteInfos.insertAt(_stringToEntity[SPAWN_STRONGER_ENEMY_NAME],
                         ecs::component::SpriteInfos{ENEMY_PATH, ENEMY_SPRITE_NB_PATTERN, 0, STRONGER_ENEMY_SCALE_FACTOR});
    enemySpawns.insertAt(_stringToEntity[SPAWN_STRONGER_ENEMY_NAME],
                         ecs::component::EnemySpawn{timeBeforeSpawnStrongerEnemy, strongerEnemyWaveSize, MAX_SPAWN_WIDTH, MIN_SPAWN_WIDTH,
                                                    MAX_SPAWN_HEIGHT, MIN_SPAWN_HEIGHT});
    velocities.insertAt(_stringToEntity[SPAWN_STRONGER_ENEMY_NAME], ecs::component::Velocity{ENEMY_VX, ENEMY_VY_STRAIGHT});
    lives.insertAt(_stringToEntity[SPAWN_STRONGER_ENEMY_NAME], ecs::component::Life{STRONGER_ENEMY_NB_LIVES});
    shootingRate.insertAt(_stringToEntity[SPAWN_STRONGER_ENEMY_NAME], ecs::component::ShootingRate{STRONGER_ENEMY_SHOOTING_RATE});
    soundEffectInfos.insertAt(_stringToEntity[SPAWN_STRONGER_ENEMY_NAME], ecs::component::SoundEffectInfos{EXPLOSION_SOUND_PATH});

    backgroundMusic.insertAt(_stringToEntity[MUSIC_NAME], ecs::component::BackgroundMusicInfos{BACKGROUND_MUSIC_PATH, 0.5, true});

    types.insertAt(_stringToEntity[TEAM_SCORE_NAME], ecs::component::Type{ecs::EntityType::TEXT});
    textInfos.insertAt(_stringToEntity[TEAM_SCORE_NAME], ecs::component::TextInfos{"Team_score:", 50, 100, 20, FONT_PATH, ORANGE_COLOR});
    score.insertAt(_stringToEntity[TEAM_SCORE_NAME], ecs::component::Score{});

    types.insertAt(_stringToEntity[TITLE_NAME], ecs::component::Type{ecs::EntityType::TEXT});
    textInfos.insertAt(_stringToEntity[TITLE_NAME], ecs::component::TextInfos{"RType", SCREEN_WIDTH / 2, 50, 20, FONT_PATH, ORANGE_COLOR});

    powerups.insertAt(_stringToEntity[POWERUP_NAME], ecs::component::Powerup{ecs::Poweruptype::NOPOWERUP, 3});
    types.insertAt(_stringToEntity[POWERUP_NAME], ecs::component::Type{ecs::EntityType::POWERUP_TEMPLATE});
    spriteInfos.insertAt( _stringToEntity[POWERUP_NAME], ecs::component::SpriteInfos{POWERUP_PATH, 10, 1, 0.5});
    textInfos.insertAt(_stringToEntity[POWERUP_NAME], ecs::component::TextInfos{"null", 0, 0, 20, FONT_PATH, ORANGE_COLOR});
}

void game::RtypeGameServer::addSystemsToRegistry() {
    _reg.addInitPlayerSystem(ecs::system::PlayerSpawnSystem());
    _reg.addSystem(ecs::system::FiringSystem());
    _reg.addSystem(ecs::system::PositionSystem());
    _reg.addSystem(ecs::system::EnemySpawnSystem());
    _reg.addSystem(ecs::system::EnemyFiringSystem());
    _reg.addSystem(ecs::system::CollisionSystem());
    _reg.addSystem(ecs::system::ScoreSystem());
    _reg.addSystem(ecs::system::PowerupSystem());
    _reg.addSystem(ecs::system::LifeSystem());
    _reg.addSystem(ecs::system::KillOffscreenEntitiesSystem());
}

game::GameStatus game::RtypeGameServer::gameState(ecs::Registry &) {
    return game::GameStatus::PLAYING;
}
