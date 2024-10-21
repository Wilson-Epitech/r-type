/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** RtypeGameClient
*/

#include "RtypeGameClient.hpp"

game::RtypeGameClient::RtypeGameClient(const std::string gameName) : AGame(gameName) {
    initializeGame();
}

void game::RtypeGameClient::spawnEntities() {}

void game::RtypeGameClient::registerComponents() {
    _reg.registerComponent<ecs::component::Position>();
    _reg.registerComponent<ecs::component::Velocity>();
    _reg.registerComponent<ecs::component::Drawable>();
    _reg.registerComponent<ecs::component::Shooting>();
    _reg.registerComponent<ecs::component::Type>();
    _reg.registerComponent<ecs::component::Life>();
    _reg.registerComponent<ecs::component::EnemySpawn>();
    _reg.registerComponent<ecs::component::SpriteInfos>();
    _reg.registerComponent<ecs::component::SoundEffect>();
    _reg.registerComponent<ecs::component::BackgroundMusic>();
    _reg.registerComponent<ecs::component::Score>();
    _reg.registerComponent<ecs::component::TextInfos>();
}

void game::RtypeGameClient::addComponentsToEntities() {}

void game::RtypeGameClient::addSystemsToRegistry() {
    _reg.addSystem(ecs::system::ControlSystem());
    _reg.addSystem(ecs::system::PositionSystem());
    _reg.addSystem(ecs::system::DrawSpriteSystem());
    _reg.addSystem(ecs::system::DrawLifeBarSystem());
    _reg.addSystem(ecs::system::DrawTextSystem());
    _reg.addSystem(ecs::system::SpriteAnimationSystem());
    _reg.addSystem(ecs::system::SoundSystem());
    _reg.addSystem(ecs::system::MusicSystem());
}

game::GameStatus game::RtypeGameClient::gameState(ecs::Registry &) {
    return game::GameStatus::PLAYING;
}
