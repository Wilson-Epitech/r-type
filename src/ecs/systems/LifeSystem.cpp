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

    for (std::size_t i = 0; i < lives.size(); ++i) {
        if (lives[i]) {
            if (lives[i]->nbLives > lives[i]->maxLives)
                lives[i]->maxLives = lives[i]->nbLives;
            if (soundEffectInfos[i] && lives[i]->nbLives == 0)
                soundEffectInfos[i]->hasToBePlayed = true;
            if (lives[i]->nbLives == 0)
                reg.killEntity(i);
        }
    }
}
