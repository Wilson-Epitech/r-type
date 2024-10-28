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
    SparseArray<component::BossSpawn> &bossLevel = reg.getComponents<component::BossSpawn>();

    for (std::size_t i = 0; i < lives.size(); ++i) {
        if (lives[i]) {
            if (lives[i]->nbLives > lives[i]->maxLives)
                lives[i]->maxLives = lives[i]->nbLives;
            if (soundEffectInfos[i] && lives[i]->nbLives == 0)
                soundEffectInfos[i]->hasToBePlayed = true;
            if (lives[i]->nbLives == 0)
                reg.killEntity(i);
            if (types[i]->entityType == ecs::EntityType::BOSS_LEVEL1 && lives[i]->nbLives == 0) {
                bossLevel[i]->hasBeenSlayed = true;
            }
        }
    }
}
