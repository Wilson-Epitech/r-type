/*
** R_TYPE PROJECT, 2024
** enemy_wave
** File description:
** SoundSystem
*/

#include "SoundSystem.hpp"

void ecs::system::SoundSystem::operator()(Registry &reg)
{
    SparseArray<component::SoundEffect> &soundEffect = reg.getComponents<component::SoundEffect>();

    for (std::size_t i = 0; i < soundEffect.size(); ++i) {
        if (!soundEffect[i])
            continue;
        if (soundEffect[i]->isLoaded == false && soundEffect[i]->soundPath.length() > 0) {
            if (!FileExists(soundEffect[i]->soundPath.c_str()))
                throw Error("Invalid sprite path");
            soundEffect[i]->sound = LoadSound(soundEffect[i]->soundPath.c_str());
            soundEffect[i]->isLoaded = true;
        }
    }
}