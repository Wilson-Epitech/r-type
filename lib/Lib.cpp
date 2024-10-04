/*
** R_TYPE PROJECT, 2024
** responcive_window
** File description:
** lib
*/

#include "Lib.hpp"
#include <sstream>
#include <string>

Vector2 Lib::getScreenRatio() {
    float widthScaleFactor = (float)SCREEN_WIDTH / (float)GetScreenWidth();
    float heightScaleFactor = (float)SCREEN_HEIGHT / (float)GetScreenHeight();

    return {widthScaleFactor, heightScaleFactor};
}

int Lib::getScreenHeight() {
    return SCREEN_HEIGHT;
}

int Lib::getScreenWidth() {
    return SCREEN_WIDTH;
}

bool Lib::isOnScreen(Vector2 pos) {
    if (pos.x > Lib::getScreenWidth() || pos.x < 0)
        return false;
    if (pos.y > Lib::getScreenHeight() || pos.y < 0)
        return false;
    return true;
}

double Lib::toMilliseconds(double time) {
    return time * TO_MILLISECONDS;
}

std::size_t Lib::retrievePlayerIndex(ecs::Registry &reg) {
    ecs::SparseArray<ecs::component::Type> &types = reg.getComponents<ecs::component::Type>();

    for (std::size_t i = 0; i < types.size(); ++i) {
        if (!types[i])
            continue;
        if (types[i]->entityType == ecs::EntityType::PLAYER)
            return i;
    }
    return 0;
}

std::optional<EntityData> Lib::retrievePlayerVelocityUpdates(ecs::Registry &reg) {
    static ecs::SparseArray<ecs::component::Velocity>::value_type lastPlayerVelocity;
    ecs::SparseArray<ecs::component::Velocity> &velocities = reg.getComponents<ecs::component::Velocity>();
    std::size_t playerIndex = retrievePlayerIndex(reg);
    EntityData playerData = EntityData{};

    if (playerIndex == 0 || !velocities[playerIndex])
        return std::nullopt;
    if (lastPlayerVelocity == velocities[playerIndex].value())
        return std::nullopt;
    lastPlayerVelocity = velocities[playerIndex].value();
    playerData.entityID = playerIndex;
    playerData.component = velocities[playerIndex].value();
    playerData.name = ecs::ComponentName::VELOCITY;
    return playerData;
}

DataToSend Lib::fillNewEntitiesDataToSend(ecs::Registry &reg) {
    std::list<std::size_t> newEntities = reg.getActualTurnNewEntities();
    std::optional<EntityData> playerData = retrievePlayerVelocityUpdates(reg);
    DataToSend data;

    if (playerData.has_value())
        data.entitiesData.push_back(playerData.value());
    data.deadEntities = reg.getActualTurnDeadEntities();

    for (auto &newEntity : newEntities) {
        fillEntityComponentData<ecs::component::Position>(reg, newEntity, data, ecs::ComponentName::POSITION);
        fillEntityComponentData<ecs::component::Velocity>(reg, newEntity, data, ecs::ComponentName::VELOCITY);
        fillEntityComponentData<ecs::component::Type>(reg, newEntity, data, ecs::ComponentName::TYPE);
        fillEntityComponentData<ecs::component::Life>(reg, newEntity, data, ecs::ComponentName::LIFE);
        fillEntityComponentData<ecs::component::Controllable>(reg, newEntity, data, ecs::ComponentName::CONTROLLABLE);
        fillEntityComponentData<ecs::component::SpriteInfos>(reg, newEntity, data, ecs::ComponentName::SPRITEINFOS);
        fillEntityComponentData<ecs::component::SoundEffect>(reg, newEntity, data, ecs::ComponentName::SOUNDEFFECT);
    }
    return data;
}

DataToSend Lib::fillAllEntitiesDataToSend(ecs::Registry &reg) {
    ecs::SparseArray<ecs::component::Position> &positions = reg.getComponents<ecs::component::Position>();
    ecs::SparseArray<ecs::component::Velocity> &velocities = reg.getComponents<ecs::component::Velocity>();
    ecs::SparseArray<ecs::component::Controllable> &controllables = reg.getComponents<ecs::component::Controllable>();
    ecs::SparseArray<ecs::component::Type> &types = reg.getComponents<ecs::component::Type>();
    ecs::SparseArray<ecs::component::Life> &lives = reg.getComponents<ecs::component::Life>();
    ecs::SparseArray<ecs::component::SpriteInfos> &spriteInfos = reg.getComponents<ecs::component::SpriteInfos>();
    ecs::SparseArray<ecs::component::SoundEffect > &soundEffect = reg.getComponents<ecs::component::SoundEffect>();
    DataToSend data;

    data.deadEntities = reg.getActualTurnDeadEntities();
    for (std::size_t i = 0; i < positions.size() || i < velocities.size() || i < types.size() || i < lives.size(); i++) {
        fillEntityComponentData<ecs::component::Position>(reg, i, data, ecs::ComponentName::POSITION);
        fillEntityComponentData<ecs::component::Velocity>(reg, i, data, ecs::ComponentName::VELOCITY);
        fillEntityComponentData<ecs::component::Type>(reg, i, data, ecs::ComponentName::TYPE);
        fillEntityComponentData<ecs::component::Life>(reg, i, data, ecs::ComponentName::LIFE);
        fillEntityComponentData<ecs::component::Controllable>(reg, i, data, ecs::ComponentName::CONTROLLABLE);
        fillEntityComponentData<ecs::component::SpriteInfos>(reg, i, data, ecs::ComponentName::SPRITEINFOS);
        fillEntityComponentData<ecs::component::SoundEffect>(reg, i, data, ecs::ComponentName::SOUNDEFFECT);
    }
    return data;
}

std::vector<std::pair<ecs::ComponentName, std::any>> Lib::getComponentCorrespondance() {
    std::vector<std::pair<ecs::ComponentName, std::any>> componentCorrespondance;

    componentCorrespondance.push_back(std::make_pair(ecs::ComponentName::POSITION, ecs::component::Position{}));
    componentCorrespondance.push_back(std::make_pair(ecs::ComponentName::VELOCITY, ecs::component::Velocity{}));
    componentCorrespondance.push_back(std::make_pair(ecs::ComponentName::SPRITEINFOS, ecs::component::SpriteInfos{}));
    componentCorrespondance.push_back(std::make_pair(ecs::ComponentName::CONTROLLABLE, ecs::component::Controllable{}));
    componentCorrespondance.push_back(std::make_pair(ecs::ComponentName::TYPE, ecs::component::Type{}));
    componentCorrespondance.push_back(std::make_pair(ecs::ComponentName::LIFE, ecs::component::Life{}));
    componentCorrespondance.push_back(std::make_pair(ecs::ComponentName::SOUNDEFFECT, ecs::component::SoundEffect{}));
    return componentCorrespondance;
}

std::string Lib::serializeEntityDataVector(const DataToSend &data) {
    std::ostringstream oss;

    oss << data.deadEntities.size() << '\n';
    for (const auto &deadEntity : data.deadEntities) {
        oss << deadEntity << '\n';
    }

    oss << data.entitiesData.size() << '\n';
    for (const auto &entityData : data.entitiesData) {
        oss << entityData;
    }
    return oss.str();
}

DataToSend Lib::deserializeEntityDataVector(const std::string &message) {
    std::istringstream stream(message);

    DataToSend data;

    size_t deadEntitiesNb;
    stream >> deadEntitiesNb;

    for (size_t i = 0; i < deadEntitiesNb; ++i) {
        std::size_t tmpEntity;
        stream >> tmpEntity;
        data.deadEntities.push_back(tmpEntity);
    }

    size_t entityDataNb;
    stream >> entityDataNb;

    for (size_t i = 0; i < entityDataNb; ++i) {
        EntityData entityData;
        stream >> entityData;
        data.entitiesData.push_back(entityData);
    }

    return data;
}

void Lib::updateEntities(ecs::Registry &reg, EntityData &data) {
    ecs::SparseArray<ecs::component::Drawable> &drawables = reg.getComponents<ecs::component::Drawable>();

    if (!reg.entityExist(data.entityID))
        reg.spawnEntity();
    if (data.entityID < drawables.size()) {
        if (!(drawables[data.entityID]))
            drawables.insertAt(data.entityID, ecs::component::Drawable({}));
    } else {
        drawables.insertAt(data.entityID, ecs::component::Drawable({}));
    }
    insertComponent<ecs::component::Position>(reg, data, ecs::ComponentName::POSITION);
    insertComponent<ecs::component::Velocity>(reg, data, ecs::ComponentName::VELOCITY);
    insertComponent<ecs::component::Type>(reg, data, ecs::ComponentName::TYPE);
    insertComponent<ecs::component::Life>(reg, data, ecs::ComponentName::LIFE);
    insertComponent<ecs::component::Controllable>(reg, data, ecs::ComponentName::CONTROLLABLE);
    insertComponent<ecs::component::SpriteInfos>(reg, data, ecs::ComponentName::SPRITEINFOS);
    insertComponent<ecs::component::SoundEffect>(reg, data, ecs::ComponentName::SOUNDEFFECT);
}
