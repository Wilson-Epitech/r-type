/*
** R_TYPE PROJECT, 2024
** branch
** File description:
** MusicSystem
*/

#include "MusicSystem.hpp"

void ecs::system::MusicSystem::operator()(Registry &reg) {
    SparseArray<component::BackgroundMusicInfos> &backgroundMusicInfos = reg.getComponents<component::BackgroundMusicInfos>();
    SparseArray<component::BackgroundMusicObject> &backgroundMusicObjects = reg.getComponents<component::BackgroundMusicObject>();

    loadMusics(backgroundMusicInfos, backgroundMusicObjects);
    for (std::size_t i = 0; i < backgroundMusicInfos.size(); ++i) {
        if (!backgroundMusicInfos[i])
            continue;
        if (backgroundMusicInfos[i]->hasToBePlayed) {
            SetMusicVolume(backgroundMusicObjects[i]->music, backgroundMusicInfos[i]->volume);
            UpdateMusicStream(backgroundMusicObjects[i]->music);
            if (!backgroundMusicInfos[i]->isCurrentlyPlayed) {
                PlayMusicStream(backgroundMusicObjects[i]->music);
                backgroundMusicInfos[i]->isCurrentlyPlayed = true;
            }
        }
    }
}

void ecs::system::MusicSystem::loadMusics(SparseArray<component::BackgroundMusicInfos> &backgroundMusicInfos, SparseArray<component::BackgroundMusicObject> &backgroundMusicObjects) {

    for (std::size_t i = 0; i < backgroundMusicInfos.size(); ++i) {
        if (!backgroundMusicInfos[i])
            continue;
        if (!backgroundMusicObjects[i]) {
            backgroundMusicObjects.insertAt(i, ecs::component::BackgroundMusicObject{});
        }
        if (backgroundMusicObjects[i] && !backgroundMusicInfos[i]->isLoaded && backgroundMusicInfos[i]->musicPath.length() > 0) {
            if (!FileExists(backgroundMusicInfos[i]->musicPath.c_str()))
                throw Error("Invalid music path");
            backgroundMusicObjects[i]->music = LoadMusicStream(backgroundMusicInfos[i]->musicPath.c_str());
            backgroundMusicInfos[i]->isLoaded = true;
        }
    }
}
