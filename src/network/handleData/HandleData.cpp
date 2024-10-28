/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** HandleData
*/

#include "HandleData.hpp"

void network::HandleData::addEntityDataToSend(std::vector<EntityData> entityData, DataToSend &data) {
    for (auto &dataToSend : entityData)
        data.entitiesData.push_back(dataToSend);
}

DataToSend network::HandleData::fillNewEntitiesDataToSend(ecs::Registry &reg) {
    std::list<entity_t> newEntities = reg.getActualTurnNewEntities();
    std::vector<EntityData> velocitiesData = retrieveComponentUpdates<ecs::component::Velocity>(reg, ecs::ComponentName::VELOCITY);
    std::vector<EntityData> shootingsData = retrieveComponentUpdates<ecs::component::Shooting>(reg, ecs::ComponentName::SHOOTING);
    std::vector<EntityData> livesData = retrieveComponentUpdates<ecs::component::Life>(reg, ecs::ComponentName::LIFE);
    std::vector<EntityData> scoresData = retrieveComponentUpdates<ecs::component::Score>(reg, ecs::ComponentName::SCORE);
    std::vector<EntityData> textInfosData = retrieveComponentUpdates<ecs::component::TextInfos>(reg, ecs::ComponentName::TEXTINFOS);
    std::vector<EntityData> activePowerupInfosData = retrieveComponentUpdates<ecs::component::ActivePowerup>(reg, ecs::ComponentName::ACTIVEPOWERUP);
    DataToSend data;

    data.deadEntities = reg.getActualTurnDeadEntities();
    network::HandleData::addEntityDataToSend(velocitiesData, data);
    network::HandleData::addEntityDataToSend(shootingsData, data);
    network::HandleData::addEntityDataToSend(livesData, data);
    network::HandleData::addEntityDataToSend(scoresData, data);
    network::HandleData::addEntityDataToSend(textInfosData, data);
    network::HandleData::addEntityDataToSend(activePowerupInfosData, data);
    for (auto &newEntity : newEntities) {
        fillEntityComponentData<ecs::component::Position>(reg, newEntity, data, ecs::ComponentName::POSITION);
        fillEntityComponentData<ecs::component::Velocity>(reg, newEntity, data, ecs::ComponentName::VELOCITY);
        fillEntityComponentData<ecs::component::Type>(reg, newEntity, data, ecs::ComponentName::TYPE);
        fillEntityComponentData<ecs::component::Life>(reg, newEntity, data, ecs::ComponentName::LIFE);
        fillEntityComponentData<ecs::component::Shooting>(reg, newEntity, data, ecs::ComponentName::SHOOTING);
        fillEntityComponentData<ecs::component::SpriteInfos>(reg, newEntity, data, ecs::ComponentName::SPRITEINFOS);
        fillEntityComponentData<ecs::component::SoundEffectInfos>(reg, newEntity, data, ecs::ComponentName::SOUNDEFFECT);
        fillEntityComponentData<ecs::component::BackgroundMusicInfos>(reg, newEntity, data, ecs::ComponentName::MUSIC);
        fillEntityComponentData<ecs::component::Score>(reg, newEntity, data, ecs::ComponentName::SCORE);
        fillEntityComponentData<ecs::component::TextInfos>(reg, newEntity, data, ecs::ComponentName::TEXTINFOS);
        fillEntityComponentData<ecs::component::ActivePowerup>(reg, newEntity, data, ecs::ComponentName::ACTIVEPOWERUP);
        fillEntityComponentData<ecs::component::MovementPatern>(reg, newEntity, data, ecs::ComponentName::MOVEMENTPATERN);
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
    for (std::size_t i = 0; i < positions.size() || i < velocities.size() || i < types.size() || i < lives.size() || i < textInfos.size();
         i++) {
        fillEntityComponentData<ecs::component::Position>(reg, i, data, ecs::ComponentName::POSITION);
        fillEntityComponentData<ecs::component::Velocity>(reg, i, data, ecs::ComponentName::VELOCITY);
        fillEntityComponentData<ecs::component::Type>(reg, i, data, ecs::ComponentName::TYPE);
        fillEntityComponentData<ecs::component::Life>(reg, i, data, ecs::ComponentName::LIFE);
        fillEntityComponentData<ecs::component::Shooting>(reg, i, data, ecs::ComponentName::SHOOTING);
        fillEntityComponentData<ecs::component::SpriteInfos>(reg, i, data, ecs::ComponentName::SPRITEINFOS);
        fillEntityComponentData<ecs::component::SoundEffectInfos>(reg, i, data, ecs::ComponentName::SOUNDEFFECT);
        fillEntityComponentData<ecs::component::BackgroundMusicInfos>(reg, i, data, ecs::ComponentName::MUSIC);
        fillEntityComponentData<ecs::component::Score>(reg, i, data, ecs::ComponentName::SCORE);
        fillEntityComponentData<ecs::component::TextInfos>(reg, i, data, ecs::ComponentName::TEXTINFOS);
        fillEntityComponentData<ecs::component::ActivePowerup>(reg, i, data, ecs::ComponentName::ACTIVEPOWERUP);
        fillEntityComponentData<ecs::component::MovementPatern>(reg, i, data, ecs::ComponentName::MOVEMENTPATERN);
    }
    return data;
}

std::unordered_map<ecs::ComponentName, std::any> network::HandleData::getComponentCorrespondence() {
    std::unordered_map<ecs::ComponentName, std::any> componentCorrespondence;

    componentCorrespondence.insert({ecs::ComponentName::POSITION, ecs::component::Position{}});
    componentCorrespondence.insert({ecs::ComponentName::VELOCITY, ecs::component::Velocity{}});
    componentCorrespondence.insert({ecs::ComponentName::SPRITEINFOS, ecs::component::SpriteInfos{}});
    componentCorrespondence.insert({ecs::ComponentName::SHOOTING, ecs::component::Shooting{}});
    componentCorrespondence.insert({ecs::ComponentName::TYPE, ecs::component::Type{}});
    componentCorrespondence.insert({ecs::ComponentName::LIFE, ecs::component::Life{}});
    componentCorrespondence.insert({ecs::ComponentName::SOUNDEFFECT, ecs::component::SoundEffectInfos{}});
    componentCorrespondence.insert({ecs::ComponentName::MUSIC, ecs::component::BackgroundMusicInfos{}});
    componentCorrespondence.insert({ecs::ComponentName::SCORE, ecs::component::Score{}});
    componentCorrespondence.insert({ecs::ComponentName::TEXTINFOS, ecs::component::TextInfos{}});
    componentCorrespondence.insert({ecs::ComponentName::ACTIVEPOWERUP, ecs::component::ActivePowerup{}});
    componentCorrespondence.insert({ecs::ComponentName::MOVEMENTPATERN, ecs::component::MovementPatern{}});
    return componentCorrespondence;
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

    if (!reg.entityExist(data.entityID))
        reg.spawnEntity();
    insertComponent<ecs::component::Position>(reg, data, ecs::ComponentName::POSITION);
    insertComponent<ecs::component::Velocity>(reg, data, ecs::ComponentName::VELOCITY);
    insertComponent<ecs::component::Type>(reg, data, ecs::ComponentName::TYPE);
    insertComponent<ecs::component::Life>(reg, data, ecs::ComponentName::LIFE);
    insertComponent<ecs::component::Shooting>(reg, data, ecs::ComponentName::SHOOTING);
    insertComponent<ecs::component::SpriteInfos>(reg, data, ecs::ComponentName::SPRITEINFOS);
    insertComponent<ecs::component::SoundEffectInfos>(reg, data, ecs::ComponentName::SOUNDEFFECT);
    insertComponent<ecs::component::BackgroundMusicInfos>(reg, data, ecs::ComponentName::MUSIC);
    insertComponent<ecs::component::Score>(reg, data, ecs::ComponentName::SCORE);
    insertComponent<ecs::component::TextInfos>(reg, data, ecs::ComponentName::TEXTINFOS);
    insertComponent<ecs::component::ActivePowerup>(reg, data, ecs::ComponentName::ACTIVEPOWERUP);
    insertComponent<ecs::component::MovementPatern>(reg, data, ecs::ComponentName::MOVEMENTPATERN);
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
