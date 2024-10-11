/*
** R_TYPE PROJECT, 2024
** branch
** File description:
** MusicSystem
*/

#include "MusicSystem.hpp"

void ecs::system::MusicSystem::operator()(Registry &reg) {
    SparseArray<component::BackgroundMusic> &music = reg.getComponents<component::BackgroundMusic>();

    for (std::size_t i = 0; i < music.size(); ++i) {
        if (!music[i])
            continue;
        loadMusic(i, music);
        if (music[i]->hasToBePlayed) {
            SetMusicVolume(music[i]->music, music[i]->volume);
            UpdateMusicStream(music[i]->music);
            if (!music[i]->isCurrentlyPlayed) {
                PlayMusicStream(music[i]->music);
                music[i]->isCurrentlyPlayed = true;
            }
        }
    }
}

void ecs::system::MusicSystem::loadMusic(std::size_t i, SparseArray<component::BackgroundMusic> &music) {

    if (!music[i]->isLoaded && music[i]->musicPath.length() > 0) {
        if (!FileExists(music[i]->musicPath.c_str()))
            throw Error("Invalid music path");
        music[i]->music = LoadMusicStream(music[i]->musicPath.c_str());
        music[i]->isLoaded = true;
    }
}
