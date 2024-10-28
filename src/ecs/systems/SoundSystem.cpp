/*
** R_TYPE PROJECT, 2024
** enemy_wave
** File description:
** SoundSystem
*/

#include "SoundSystem.hpp"

void ecs::system::SoundSystem::operator()(Registry &reg) {
    SparseArray<component::SoundEffectInfos> &soundEffectInfos = reg.getComponents<component::SoundEffectInfos>();
    SparseArray<component::SoundEffectObject> &soundEffectObjects = reg.getComponents<component::SoundEffectObject>();

    loadSounds(soundEffectInfos, soundEffectObjects);
    for (std::size_t i = 0; i < soundEffectInfos.size(); ++i) {
        if (!soundEffectInfos[i])
            continue;
        if (soundEffectInfos[i]->hasToBePlayed) {
            PlaySound(soundEffectObjects[i]->sound);
            soundEffectInfos[i]->hasToBePlayed = false;
        }
    }
}

void ecs::system::SoundSystem::loadSounds(SparseArray<component::SoundEffectInfos> &soundEffectInfos,
                                          SparseArray<component::SoundEffectObject> &soundEffectObjects) {

    for (std::size_t i = 0; i < soundEffectInfos.size(); ++i) {
        if (!soundEffectInfos[i])
            continue;
        if (!soundEffectObjects[i]) {
            soundEffectObjects.insertAt(i, ecs::component::SoundEffectObject{});
        }
        if (soundEffectObjects[i] && !soundEffectInfos[i]->isLoaded && soundEffectInfos[i]->soundPath.length() > 0) {
            if (!FileExists(soundEffectInfos[i]->soundPath.c_str()))
                throw Error("Invalid song path");
            soundEffectObjects[i]->sound = LoadSound(soundEffectInfos[i]->soundPath.c_str());
            soundEffectInfos[i]->isLoaded = true;
        }
    }
}
