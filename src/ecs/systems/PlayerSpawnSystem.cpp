/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** PlayerSpawnSystem
*/

#include "PlayerSpawnSystem.hpp"

void ecs::system::PlayerSpawnSystem::operator()(Registry &reg) {

    std::map<std::size_t, entity_t> players = reg.getPlayers();

    for (auto &it : players)
        createNewPlayer(reg, it.second);
}

void ecs::system::PlayerSpawnSystem::createNewPlayer(Registry &reg, const entity_t &entity) {
    ecs::SparseArray<ecs::component::Position> &positions = reg.getComponents<ecs::component::Position>();
    ecs::SparseArray<ecs::component::Velocity> &velocities = reg.getComponents<ecs::component::Velocity>();
    ecs::SparseArray<ecs::component::Shooting> &shootings = reg.getComponents<ecs::component::Shooting>();
    ecs::SparseArray<ecs::component::Type> &types = reg.getComponents<ecs::component::Type>();
    ecs::SparseArray<ecs::component::Life> &lives = reg.getComponents<ecs::component::Life>();
    ecs::SparseArray<ecs::component::SpriteInfos> &spriteInfos = reg.getComponents<ecs::component::SpriteInfos>();
    ecs::SparseArray<ecs::component::SoundEffectInfos> &soundEffectInfos = reg.getComponents<ecs::component::SoundEffectInfos>();
    ecs::SparseArray<ecs::component::ShootingRate> &shootingRate = reg.getComponents<ecs::component::ShootingRate>();
    ecs::SparseArray<ecs::component::Score> &score = reg.getComponents<ecs::component::Score>();
    ecs::SparseArray<ecs::component::TextInfos> &textInfos = reg.getComponents<ecs::component::TextInfos>();
    ecs::SparseArray<ecs::component::ActivePowerup> &activePowerup = reg.getComponents<ecs::component::ActivePowerup>();
    std::size_t playerTemplateIndex = getPlayerTemplateIndex(reg);

    if (playerTemplateIndex == 0)
        return;

    if (!types[entity])
        types.insertAt(entity, ecs::component::Type{ecs::EntityType::PLAYER});
    if (!positions[entity])
        positions.insertAt(entity, ecs::component::Position{100, 100});
    if (velocities[playerTemplateIndex] && !velocities[entity])
        velocities.insertAt(entity, ecs::component::Velocity{velocities[playerTemplateIndex].value()});
    if (shootings[playerTemplateIndex] && !shootings[entity])
        shootings.insertAt(entity, ecs::component::Shooting{shootings[playerTemplateIndex].value()});
    if (lives[playerTemplateIndex] && !lives[entity])
        lives.insertAt(entity, ecs::component::Life{lives[playerTemplateIndex].value()});
    if (spriteInfos[playerTemplateIndex] && !spriteInfos[entity]) {
        spriteInfos.insertAt(entity, ecs::component::SpriteInfos{spriteInfos[playerTemplateIndex].value()});
        setPlayerColor(reg, entity);
    }
    if (soundEffectInfos[playerTemplateIndex] && !soundEffectInfos[entity])
        soundEffectInfos.insertAt(entity, ecs::component::SoundEffectInfos{soundEffectInfos[playerTemplateIndex].value()});
    if (shootingRate[playerTemplateIndex] && !shootingRate[entity])
        shootingRate.insertAt(entity, ecs::component::ShootingRate{shootingRate[playerTemplateIndex].value()});
    if (score[playerTemplateIndex] && !score[entity])
        score.insertAt(entity, ecs::component::Score{score[playerTemplateIndex].value()});
    if (textInfos[playerTemplateIndex] && !textInfos[entity])
        textInfos.insertAt(entity, ecs::component::TextInfos{textInfos[playerTemplateIndex].value()});
    if (activePowerup[playerTemplateIndex] && !activePowerup[entity])
        activePowerup.insertAt(entity, ecs::component::ActivePowerup{activePowerup[playerTemplateIndex].value()});
}

std::size_t ecs::system::PlayerSpawnSystem::getPlayerTemplateIndex(Registry &reg) {

    ecs::SparseArray<ecs::component::Type> &types = reg.getComponents<ecs::component::Type>();

    for (std::size_t i = 0; i < types.size(); ++i) {
        if (types[i]) {
            if (types[i]->entityType == ecs::EntityType::PLAYER_TEMPLATE) {
                return i;
            }
        }
    }
    return 0;
}

void ecs::system::PlayerSpawnSystem::setPlayerColor(Registry &reg, const entity_t &entity) {
    ecs::SparseArray<ecs::component::SpriteInfos> &spriteInfos = reg.getComponents<ecs::component::SpriteInfos>();
    std::vector<ecs::CustomColor> availableSpaceshipColors = {CYAN_COLOR, BEIGE_COLOR, GOLD_COLOR, PINK_COLOR};
    std::vector<std::pair<entity_t, ecs::CustomColor>> playersColors = reg.getPlayersColors();

    for (auto availableSpaceshipColor : availableSpaceshipColors) {
        auto it = std::find_if(playersColors.begin(), playersColors.end(),
                               [availableSpaceshipColor](const auto &pair) { return ((pair.second == availableSpaceshipColor)); });
        if (it == playersColors.end()) {
            spriteInfos[entity]->color = availableSpaceshipColor;
            reg.addPlayerColor(entity, availableSpaceshipColor);
            break;
        }
    }
}
