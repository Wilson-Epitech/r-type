/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** LifeSystem
*/

#include "LifeSystem.hpp"

void ecs::system::LifeSystem::operator()(Registry &reg) {
    SparseArray<component::Life> &lives = reg.getComponents<component::Life>();
    SparseArray<component::SoundEffectInfos> &soundEffectInfos = reg.getComponents<component::SoundEffectInfos>();
    SparseArray<component::Type> &types = reg.getComponents<component::Type>();
    SparseArray<component::BossSpawn> &bossSpawns = reg.getComponents<component::BossSpawn>();

    for (std::size_t i = 0; i < lives.size() && i < types.size(); ++i) {
        if (!lives[i] || !types[i])
            continue;
        if (lives[i]->nbLives > lives[i]->maxLives)
            lives[i]->maxLives = lives[i]->nbLives;
        if (i < soundEffectInfos.size()) {
            if (soundEffectInfos[i] && lives[i]->nbLives == 0)
                soundEffectInfos[i]->hasToBePlayed = true;
        }
        if (lives[i]->nbLives == 0)
            reg.killEntity(i);
        if (types[i]->entityType == ecs::EntityType::BOSS_LEVEL1 && lives[i]->nbLives == 0) {
            bossSpawns[i]->hasBeenSlayed = true;
            bossSpawns[findLevel1BossEntity(reg)]->ongoingLevel = CurrentLevel::LEVEL2;
        }
    }
}

std::size_t ecs::system::LifeSystem::findLevel1BossEntity(Registry &reg) {
    SparseArray<component::Type> &types = reg.getComponents<component::Type>();

    for (std::size_t i = 0; i < types.size(); i++) {
        if (types[i]->entityType == ecs::EntityType::BOSS_LEVEL1)
            return i;
    }
    return 0;
}
