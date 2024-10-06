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

void ecs::system::PlayerSpawnSystem::createNewPlayer(Registry &reg, entity_t entity)
{
    ecs::SparseArray<ecs::component::Position> &positions = reg.getComponents<ecs::component::Position>();
    ecs::SparseArray<ecs::component::Velocity> &velocities = reg.getComponents<ecs::component::Velocity>();
    ecs::SparseArray<ecs::component::Controllable> &controllables = reg.getComponents<ecs::component::Controllable>();
    ecs::SparseArray<ecs::component::Type> &types = reg.getComponents<ecs::component::Type>();
    ecs::SparseArray<ecs::component::Life> &lives = reg.getComponents<ecs::component::Life>();
    ecs::SparseArray<ecs::component::SpriteInfos> &spriteInfos = reg.getComponents<ecs::component::SpriteInfos>();
    ecs::SparseArray<ecs::component::SoundEffect > &soundEffect = reg.getComponents<ecs::component::SoundEffect>();
    std::size_t playerTemplateIndex = getPlayerTemplateIndex(reg);

    if (playerTemplateIndex == 0)
        return;

    if (!types[entity])
        types.insertAt(entity, ecs::component::Type{ecs::EntityType::PLAYER});
    if (!positions[entity])
        positions.insertAt(entity, ecs::component::Position{100, 100});
    if (velocities[playerTemplateIndex] && !velocities[entity])
        velocities.insertAt(entity, ecs::component::Velocity{velocities[playerTemplateIndex].value()});
    if (controllables[playerTemplateIndex] && !controllables[entity])
        controllables.insertAt(entity, ecs::component::Controllable{controllables[playerTemplateIndex].value()});
    if (lives[playerTemplateIndex] && !lives[entity])
        lives.insertAt(entity, ecs::component::Life{lives[playerTemplateIndex].value()});
    if (spriteInfos[playerTemplateIndex] && !spriteInfos[entity])
        spriteInfos.insertAt(entity, ecs::component::SpriteInfos{spriteInfos[playerTemplateIndex].value()});
    if (soundEffect[playerTemplateIndex] && !soundEffect[entity])
        soundEffect.insertAt(entity, ecs::component::SoundEffect{soundEffect[playerTemplateIndex].value()});
}

std::size_t ecs::system::PlayerSpawnSystem::getPlayerTemplateIndex(Registry &reg)
{

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
