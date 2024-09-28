/*
** R_TYPE PROJECT, 2024
** enemy_wave
** File description:
** SoundSystem
*/

#include "SoundSystem.hpp"

void ecs::system::SoundSystem::operator()(Registry &reg) {
    SparseArray<component::SoundEffect> &soundEffect = reg.getComponents<component::SoundEffect>();

    for (std::size_t i = 0; i < soundEffect.size(); ++i) {
        if (!soundEffect[i])
            continue;
        loadSound(i, reg);
        playDeadSong(i, reg);
        if (soundEffect[i]->hasToBePlayed) {
            PlaySound(soundEffect[i]->sound);
            soundEffect[i]->hasToBePlayed = false;
        }
    }
}

void ecs::system::SoundSystem::playDeadSong(std::size_t i, Registry &reg) {
    SparseArray<component::SoundEffect> &soundEffect = reg.getComponents<component::SoundEffect>();
    SparseArray<component::Life> &lives = reg.getComponents<component::Life>();

    if (lives[i]) {
        if (lives[i]->nbLives == 0) {
            soundEffect[i]->hasToBePlayed = true;
        }
    }
}

void ecs::system::SoundSystem::loadSound(std::size_t i, Registry &reg) {
    SparseArray<component::SoundEffect> &soundEffect = reg.getComponents<component::SoundEffect>();

    if (!soundEffect[i]->isLoaded && soundEffect[i]->soundPath.length() > 0) {
        if (!FileExists(soundEffect[i]->soundPath.c_str()))
            throw Error("Invalid song path");
        soundEffect[i]->sound = LoadSound(soundEffect[i]->soundPath.c_str());
        soundEffect[i]->isLoaded = true;
    }
}
