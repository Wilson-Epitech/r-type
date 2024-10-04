/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** LifeSystem
*/

#include "LifeSystem.hpp"

void ecs::system::LifeSystem::operator()(Registry &reg) {
    SparseArray<component::Life> &lives = reg.getComponents<component::Life>();
    SparseArray<component::SoundEffect> &soundEffect = reg.getComponents<component::SoundEffect>();

    for (std::size_t i = 0; i < lives.size(); ++i) {
        if (lives[i]) {
            if (soundEffect[i] && lives[i]->nbLives == 0)
                soundEffect[i]->hasToBePlayed = true;
            if (lives[i]->nbLives == 0)
                reg.killEntity(i);
        }
    }
}
