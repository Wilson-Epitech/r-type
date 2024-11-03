/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** BossSpawnSystem
*/

#include "BossSpawnSystem.hpp"

void ecs::system::BossSpawnSystem::operator()(Registry &reg) {
    SparseArray<component::Type> &types = reg.getComponents<component::Type>();
    SparseArray<component::Score> &scores = reg.getComponents<component::Score>();
    SparseArray<component::Position> &positions = reg.getComponents<component::Position>();
    SparseArray<component::Velocity> &velocities = reg.getComponents<component::Velocity>();
    SparseArray<component::BossSpawn> &bossSpawns = reg.getComponents<component::BossSpawn>();
    SparseArray<component::BackgroundMusicInfos> &musics = reg.getComponents<component::BackgroundMusicInfos>();
    static std::chrono::steady_clock::time_point time0 = std::chrono::steady_clock::now();
    static bool isTimerSet = false;
    static bool bossSpawned = false;

    for (std::size_t i = 0; i < types.size() && i < scores.size() && i < positions.size() && i < velocities.size(); ++i) {
        if (!types[i])
            continue;
        if (types[i]->entityType == ecs::EntityType::TEXT && scores[i]->score == SCORE_FINAL_BOSS_LEVEL1 && !isTimerSet) {
            if (!isTimerSet) {
                time0 = std::chrono::steady_clock::now();
                isTimerSet = true;
            }
        }
        if (isTimerSet && !bossSpawned) {
            if (Lib::isTimeElapsed(time0, TIME_WAIT_LEVEL1_BOSS)) {
                std::size_t bossEntity = findLevel1BossEntity(types);
                if (bossEntity != 0) {
                    createNewBoss(reg, types, bossEntity);
                    bossSpawned = true;
                    if (bossEntity < bossSpawns.size()) {
                        if (bossSpawns[bossEntity]) {
                            bossSpawns[bossEntity]->ongoingLevel = CurrentLevel::LEVEL1_BOSS;
                            musics[findBackgroundMusic(reg)]->isLoaded = false;
                            musics[findBackgroundMusic(reg)]->musicPath = BACKGROUND_MUSIC_PATH_BOSS1;
                            musics[findBackgroundMusic(reg)]->isCurrentlyPlayed = false;
                        }
                    }
                }
            }
        }
        if (!positions[i])
            continue;
        if (bossSpawned && types[i]->entityType == ecs::EntityType::BOSS_LEVEL1 && positions[i]->x <= LEVEL1_BOSS_STOP) {
            if (velocities[i])
                velocities[i]->vx = 0;
        }
    }
}

void ecs::system::BossSpawnSystem::createNewBoss(Registry &reg, SparseArray<component::Type> &types, const std::size_t &i) {
    SparseArray<component::Velocity> &velocities = reg.getComponents<component::Velocity>();
    SparseArray<component::Life> &lives = reg.getComponents<component::Life>();
    SparseArray<component::SpriteInfos> &spriteInfos = reg.getComponents<component::SpriteInfos>();
    SparseArray<component::Position> &positions = reg.getComponents<component::Position>();
    SparseArray<component::ShootingRate> &shootingRate = reg.getComponents<component::ShootingRate>();
    SparseArray<component::SoundEffectInfos> &soundEffect = reg.getComponents<component::SoundEffectInfos>();

    entity_t entity = reg.spawnEntity();

    positions.insertAt(entity, ecs::component::Position{LEVEL1_BOSS_SPAWN_X, LEVEL1_BOSS_SPAWN_Y});
    velocities.insertAt(entity, ecs::component::Velocity{LEVEL1_BOSS_VELOCITY, 0, 0});
    spriteInfos.insertAt(entity, ecs::component::SpriteInfos{spriteInfos[i]->spritePath, spriteInfos[i]->patternNb,
                                                             spriteInfos[i]->firstPattern, spriteInfos[i]->scaleFactor});
    types.insertAt(entity, ecs::component::Type{ecs::EntityType::BOSS_LEVEL1});
    lives.insertAt(entity, ecs::component::Life{lives[i]->nbLives, lives[i]->displayLife, lives[i]->hasToSummonPowerupAndWeapon});
    if (shootingRate.size() > i) {
        if (shootingRate[i])
            shootingRate.insertAt(entity, ecs::component::ShootingRate{shootingRate[i]->shootRate});
    }
    if (soundEffect.size() > i) {
        if (soundEffect[i])
            soundEffect.insertAt(entity, ecs::component::SoundEffectInfos{soundEffect[i]->soundPath});
    }
}

std::size_t ecs::system::BossSpawnSystem::findLevel1BossEntity(SparseArray<component::Type> &types) {
    for (std::size_t i = 0; i < types.size(); i++) {
        if (types[i]->entityType == ecs::EntityType::BOSS_LEVEL1)
            return i;
    }
    return 0;
}

std::size_t ecs::system::BossSpawnSystem::findBackgroundMusic(Registry &reg) {
    SparseArray<component::BackgroundMusicInfos> &musics = reg.getComponents<component::BackgroundMusicInfos>();

    for (std::size_t i = 0; i < musics.size(); i++) {
        if (!musics[i])
            continue;
        return i;
    }
    return 0;
}