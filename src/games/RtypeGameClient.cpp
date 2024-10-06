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
    _reg.registerComponent<ecs::component::Controllable>();
    _reg.registerComponent<ecs::component::Type>();
    _reg.registerComponent<ecs::component::Life>();
    _reg.registerComponent<ecs::component::EnemySpawn>();
    _reg.registerComponent<ecs::component::SpriteInfos>();
    _reg.registerComponent<ecs::component::SoundEffect>();
}

void game::RtypeGameClient::addComponentsToEntities() {}

void game::RtypeGameClient::addSystemsToRegistry() {
    _reg.addSystem(ecs::system::ControlSystem());
    _reg.addSystem(ecs::system::PositionSystem());
    _reg.addSystem(ecs::system::DrawSystem());
    _reg.addSystem(ecs::system::SpriteAnimationSystem());
    _reg.addSystem(ecs::system::SoundSystem());
}

game::GameStatus game::RtypeGameClient::gameState(ecs::Registry &) {
    return game::GameStatus::PLAYING;
}
