/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** HandleData
*/

#include "HandleData.hpp"

std::vector<std::size_t> network::HandleData::retrievePlayersIndex(ecs::Registry &reg) {
    ecs::SparseArray<ecs::component::Type> &types = reg.getComponents<ecs::component::Type>();
    std::map<std::size_t, entity_t> players = reg.getPlayers();
    std::vector<std::size_t> playersIndex;

    for (std::size_t i = 0; i < types.size(); ++i) {
        if (!types[i])
            continue;
        if (types[i]->entityType == ecs::EntityType::PLAYER) {
            for (auto &player : players) {
                if (player.second == i)
                    playersIndex.push_back(i);
            }
        }
    }
    return playersIndex;
}

std::vector<std::size_t> network::HandleData::retrieveTextIndex(ecs::Registry &reg) {
    ecs::SparseArray<ecs::component::Type> &types = reg.getComponents<ecs::component::Type>();
    ecs::SparseArray<ecs::component::TextInfos> &textInfos = reg.getComponents<ecs::component::TextInfos>();
    std::vector<std::size_t> textIndex;

    for (std::size_t i = 0; i < types.size() && i < textInfos.size(); ++i) {
        if (!types[i] || !textInfos[i])
            continue;
        if (types[i]->entityType == ecs::EntityType::TEXT && textInfos[i]->hasChanged) {
            textInfos[i]->hasChanged = false;
            textIndex.push_back(i);
        }
    }
    return textIndex;
}

void network::HandleData::addEntityDataToSend(std::vector<EntityData> entityData, DataToSend &data) {
    for (auto &dataToSend : entityData)
        data.entitiesData.push_back(dataToSend);
}

DataToSend network::HandleData::fillNewEntitiesDataToSend(ecs::Registry &reg) {
    std::list<std::size_t> newEntities = reg.getActualTurnNewEntities();
    std::vector<EntityData> playersVelocitiesData = retrieveComponentUpdates<ecs::component::Velocity>(reg, ecs::ComponentName::VELOCITY, retrievePlayersIndex);
    std::vector<EntityData> playersShootingsData = retrieveComponentUpdates<ecs::component::Shooting>(reg, ecs::ComponentName::SHOOTING, retrievePlayersIndex);
    std::vector<EntityData> playersLivesData = retrieveComponentUpdates<ecs::component::Life>(reg, ecs::ComponentName::LIFE, retrievePlayersIndex);
    std::vector<EntityData> playersScoresData = retrieveComponentUpdates<ecs::component::Score>(reg, ecs::ComponentName::SCORE, retrievePlayersIndex);
    std::vector<EntityData> playersTextInfosData = retrieveComponentUpdates<ecs::component::TextInfos>(reg, ecs::ComponentName::TEXTINFOS, retrievePlayersIndex);
    std::vector<EntityData> teamScoresInfosData = retrieveComponentUpdates<ecs::component::TextInfos>(reg, ecs::ComponentName::TEXTINFOS, retrieveTextIndex);

    DataToSend data;

    data.deadEntities = reg.getActualTurnDeadEntities();
    network::HandleData::addEntityDataToSend(playersVelocitiesData, data);
    network::HandleData::addEntityDataToSend(playersShootingsData, data);
    network::HandleData::addEntityDataToSend(playersLivesData, data);
    network::HandleData::addEntityDataToSend(playersScoresData, data);
    network::HandleData::addEntityDataToSend(playersTextInfosData, data);
    network::HandleData::addEntityDataToSend(teamScoresInfosData, data);
    for (auto &newEntity : newEntities) {
        fillEntityComponentData<ecs::component::Position>(reg, newEntity, data, ecs::ComponentName::POSITION);
        fillEntityComponentData<ecs::component::Velocity>(reg, newEntity, data, ecs::ComponentName::VELOCITY);
        fillEntityComponentData<ecs::component::Type>(reg, newEntity, data, ecs::ComponentName::TYPE);
        fillEntityComponentData<ecs::component::Life>(reg, newEntity, data, ecs::ComponentName::LIFE);
        fillEntityComponentData<ecs::component::Shooting>(reg, newEntity, data, ecs::ComponentName::SHOOTING);
        fillEntityComponentData<ecs::component::SpriteInfos>(reg, newEntity, data, ecs::ComponentName::SPRITEINFOS);
        fillEntityComponentData<ecs::component::SoundEffect>(reg, newEntity, data, ecs::ComponentName::SOUNDEFFECT);
        fillEntityComponentData<ecs::component::BackgroundMusic>(reg, newEntity, data, ecs::ComponentName::MUSIC);
        fillEntityComponentData<ecs::component::Score>(reg, newEntity, data, ecs::ComponentName::SCORE);
        fillEntityComponentData<ecs::component::TextInfos>(reg, newEntity, data, ecs::ComponentName::TEXTINFOS);
    }
    return data;
}

DataToSend network::HandleData::fillAllEntitiesDataToSend(ecs::Registry &reg) {
    ecs::SparseArray<ecs::component::Position> &positions = reg.getComponents<ecs::component::Position>();
    ecs::SparseArray<ecs::component::Velocity> &velocities = reg.getComponents<ecs::component::Velocity>();
    ecs::SparseArray<ecs::component::Type> &types = reg.getComponents<ecs::component::Type>();
    ecs::SparseArray<ecs::component::Life> &lives = reg.getComponents<ecs::component::Life>();
    ecs::SparseArray<ecs::component::TextInfos> &textInfos = reg.getComponents<ecs::component::TextInfos>();
    DataToSend data;

    data.deadEntities = reg.getActualTurnDeadEntities();
    for (std::size_t i = 0; i < positions.size() || i < velocities.size() || i < types.size() || i < lives.size() || i < textInfos.size(); i++) {
        fillEntityComponentData<ecs::component::Position>(reg, i, data, ecs::ComponentName::POSITION);
        fillEntityComponentData<ecs::component::Velocity>(reg, i, data, ecs::ComponentName::VELOCITY);
        fillEntityComponentData<ecs::component::Type>(reg, i, data, ecs::ComponentName::TYPE);
        fillEntityComponentData<ecs::component::Life>(reg, i, data, ecs::ComponentName::LIFE);
        fillEntityComponentData<ecs::component::Shooting>(reg, i, data, ecs::ComponentName::SHOOTING);
        fillEntityComponentData<ecs::component::SpriteInfos>(reg, i, data, ecs::ComponentName::SPRITEINFOS);
        fillEntityComponentData<ecs::component::SoundEffect>(reg, i, data, ecs::ComponentName::SOUNDEFFECT);
        fillEntityComponentData<ecs::component::BackgroundMusic>(reg, i, data, ecs::ComponentName::MUSIC);
        fillEntityComponentData<ecs::component::Score>(reg, i, data, ecs::ComponentName::SCORE);
        fillEntityComponentData<ecs::component::TextInfos>(reg, i, data, ecs::ComponentName::TEXTINFOS);
    }
    return data;
}

std::vector<std::pair<ecs::ComponentName, std::any>> network::HandleData::getComponentCorrespondance() {
    std::vector<std::pair<ecs::ComponentName, std::any>> componentCorrespondance;

    componentCorrespondance.push_back(std::make_pair(ecs::ComponentName::POSITION, ecs::component::Position{}));
    componentCorrespondance.push_back(std::make_pair(ecs::ComponentName::VELOCITY, ecs::component::Velocity{}));
    componentCorrespondance.push_back(std::make_pair(ecs::ComponentName::SPRITEINFOS, ecs::component::SpriteInfos{}));
    componentCorrespondance.push_back(std::make_pair(ecs::ComponentName::SHOOTING, ecs::component::Shooting{}));
    componentCorrespondance.push_back(std::make_pair(ecs::ComponentName::TYPE, ecs::component::Type{}));
    componentCorrespondance.push_back(std::make_pair(ecs::ComponentName::LIFE, ecs::component::Life{}));
    componentCorrespondance.push_back(std::make_pair(ecs::ComponentName::SOUNDEFFECT, ecs::component::SoundEffect{}));
    componentCorrespondance.push_back(std::make_pair(ecs::ComponentName::MUSIC, ecs::component::BackgroundMusic{}));
    componentCorrespondance.push_back(std::make_pair(ecs::ComponentName::SCORE, ecs::component::Score{}));
    componentCorrespondance.push_back(std::make_pair(ecs::ComponentName::TEXTINFOS, ecs::component::TextInfos{}));
    return componentCorrespondance;
}

std::string network::HandleData::serializeEntityDataVector(const DataToSend &data) {
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

DataToSend network::HandleData::deserializeEntityDataVector(const std::string &message) {
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
        EntityData entityData = EntityData{};
        stream >> entityData;
        data.entitiesData.push_back(entityData);
    }

    return data;
}

void network::HandleData::updateEntity(ecs::Registry &reg, EntityData &data) {
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
    insertComponent<ecs::component::Shooting>(reg, data, ecs::ComponentName::SHOOTING);
    insertComponent<ecs::component::SpriteInfos>(reg, data, ecs::ComponentName::SPRITEINFOS);
    insertComponent<ecs::component::SoundEffect>(reg, data, ecs::ComponentName::SOUNDEFFECT);
    insertComponent<ecs::component::BackgroundMusic>(reg, data, ecs::ComponentName::MUSIC);
    insertComponent<ecs::component::Score>(reg, data, ecs::ComponentName::SCORE);
    insertComponent<ecs::component::TextInfos>(reg, data, ecs::ComponentName::TEXTINFOS);
}

void network::HandleData::updateDeadEntities(DataToSend &dataReceived, ecs::Registry &reg) {
    for (auto &deadEntity : dataReceived.deadEntities) {
        if (reg.entityExist(deadEntity))
            reg.killEntity(deadEntity);
    }
}

void network::HandleData::updateEntitiesData(DataToSend &dataReceived, ecs::Registry &reg) {
    for (auto &entity : dataReceived.entitiesData) {
        if (entity.entityID != 0)
            updateEntity(reg, entity);
    }
}

void network::HandleData::deleteSimilarDataLines(DataToSend &dataToSend, const DataToSend &dataReceived) {
    for (auto dataToSendIt = dataToSend.deadEntities.begin(); dataToSendIt != dataToSend.deadEntities.end(); dataToSendIt++) {
        for (auto dataReceivedIt = dataReceived.deadEntities.begin(); dataReceivedIt != dataReceived.deadEntities.end(); dataReceivedIt++) {
            if (*dataToSendIt == *dataReceivedIt) {
                dataToSendIt = dataToSend.deadEntities.erase(dataToSendIt);
                dataToSendIt--;
            }
        }
    }
    for (auto dataToSendIt = dataToSend.entitiesData.begin(); dataToSendIt != dataToSend.entitiesData.end(); dataToSendIt++) {
        for (auto dataReceivedIt = dataReceived.entitiesData.begin(); dataReceivedIt != dataReceived.entitiesData.end(); dataReceivedIt++) {
            if (*dataToSendIt == *dataReceivedIt) {
                dataToSendIt = dataToSend.entitiesData.erase(dataToSendIt);
                dataToSendIt--;
            }
        }
    }
}
