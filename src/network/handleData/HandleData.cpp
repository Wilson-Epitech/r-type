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

void network::HandleData::sendEntitiesDataForNewLevel(ecs::Registry &reg, DataToSend &data) {
    std::vector<EntityData> positionData = retrieveComponentUpdates<ecs::component::Position>(reg, ecs::ComponentName::POSITION);
    std::vector<EntityData> spriteInfoData = retrieveComponentUpdates<ecs::component::SpriteInfos>(reg, ecs::ComponentName::SPRITEINFOS);
    ecs::SparseArray<ecs::component::Shooting> &shootings = reg.getComponents<ecs::component::Shooting>();
    ecs::SparseArray<ecs::component::Type> &types = reg.getComponents<ecs::component::Type>();
    ecs::SparseArray<ecs::component::BossSpawn> &bossSlayed = reg.getComponents<ecs::component::BossSpawn>();
    static std::chrono::steady_clock::time_point time0 = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point actualTime = std::chrono::steady_clock::now();
    std::chrono::milliseconds timeElapsed = std::chrono::duration_cast<std::chrono::seconds>(actualTime - time0);
    std::size_t timeRequired = 2;

    for (std::size_t i = 0; i < bossSlayed.size(); i++) {
        if (bossSlayed[i]->hasBeenSlayed && types[i]->entityType == ecs::EntityType::BOSS) {
            network::HandleData::addEntityDataToSend(positionData, data);
            network::HandleData::addEntityDataToSend(spriteInfoData, data);
            bossSlayed[i]->hasBeenSlayed = false;
        }
    }
    if (timeElapsed.count() >= timeRequired) {
        std::vector<EntityData> playersShootingData;
        std::map<std::size_t, entity_t> clientsToPlayers = reg.getPlayers();
        for (auto &clientToPlayer : clientsToPlayers) {
            if (shootings[clientToPlayer.second]) {
                EntityData playerShootingData;
                playerShootingData.name = ecs::ComponentName::SHOOTING;
                playerShootingData.entityID = clientToPlayer.second;
                playerShootingData.component = shootings[clientToPlayer.second].value();
                playersShootingData.push_back(playerShootingData);
            }
        }
        time0 = std::chrono::steady_clock::now();
        network::HandleData::addEntityDataToSend(playersShootingData, data);
    }
}

DataToSend network::HandleData::fillNewEntitiesDataToSend(ecs::Registry &reg, const bool &isServer) {
    std::list<entity_t> newEntities = reg.getActualTurnNewEntities();
    std::vector<EntityData> velocitiesData = retrieveComponentUpdates<ecs::component::Velocity>(reg, ecs::ComponentName::VELOCITY);
    std::vector<EntityData> shootingsData = retrieveComponentUpdates<ecs::component::Shooting>(reg, ecs::ComponentName::SHOOTING);
    std::vector<EntityData> livesData = retrieveComponentUpdates<ecs::component::Life>(reg, ecs::ComponentName::LIFE);
    std::vector<EntityData> scoresData = retrieveComponentUpdates<ecs::component::Score>(reg, ecs::ComponentName::SCORE);
    std::vector<EntityData> textInfosData = retrieveComponentUpdates<ecs::component::TextInfos>(reg, ecs::ComponentName::TEXTINFOS);
    std::vector<EntityData> gameOverData = retrieveComponentUpdates<ecs::component::GameOver>(reg, ecs::ComponentName::GAMEOVER);
    std::vector<EntityData> activePowerupInfosData =
        retrieveComponentUpdates<ecs::component::ActivePowerup>(reg, ecs::ComponentName::ACTIVEPOWERUP);
    std::vector<EntityData> weaponData = retrieveComponentUpdates<ecs::component::Weapon>(reg, ecs::ComponentName::PLAYERWEAPON);
    std::vector<EntityData> musicData = retrieveComponentUpdates<ecs::component::BackgroundMusicInfos>(reg, ecs::ComponentName::MUSIC);
    DataToSend data;

    data.deadEntities = reg.getActualTurnDeadEntities();
    network::HandleData::addEntityDataToSend(velocitiesData, data);
    network::HandleData::addEntityDataToSend(shootingsData, data);
    network::HandleData::addEntityDataToSend(livesData, data);
    network::HandleData::addEntityDataToSend(scoresData, data);
    network::HandleData::addEntityDataToSend(textInfosData, data);
    network::HandleData::addEntityDataToSend(gameOverData, data);
    network::HandleData::addEntityDataToSend(activePowerupInfosData, data);
    network::HandleData::addEntityDataToSend(weaponData, data);
    network::HandleData::addEntityDataToSend(musicData, data);

    if (isServer)
        sendEntitiesDataForNewLevel(reg, data);

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
        fillEntityComponentData<ecs::component::GameOver>(reg, newEntity, data, ecs::ComponentName::GAMEOVER);
        fillEntityComponentData<ecs::component::ActivePowerup>(reg, newEntity, data, ecs::ComponentName::ACTIVEPOWERUP);
        fillEntityComponentData<ecs::component::MovementPattern>(reg, newEntity, data, ecs::ComponentName::MOVEMENTPATTERN);
        fillEntityComponentData<ecs::component::Weapon>(reg, newEntity, data, ecs::ComponentName::PLAYERWEAPON);
    }
    return data;
}

DataToSend network::HandleData::fillAllEntitiesDataToSend(ecs::Registry &reg) {
    ecs::SparseArray<ecs::component::Position> &positions = reg.getComponents<ecs::component::Position>();
    ecs::SparseArray<ecs::component::Velocity> &velocities = reg.getComponents<ecs::component::Velocity>();
    ecs::SparseArray<ecs::component::Type> &types = reg.getComponents<ecs::component::Type>();
    ecs::SparseArray<ecs::component::Life> &lives = reg.getComponents<ecs::component::Life>();
    ecs::SparseArray<ecs::component::TextInfos> &textInfos = reg.getComponents<ecs::component::TextInfos>();
    ecs::SparseArray<ecs::component::GameOver> &gameOverInfos = reg.getComponents<ecs::component::GameOver>();
    DataToSend data;

    data.deadEntities = reg.getActualTurnDeadEntities();
    for (std::size_t i = 0; i < positions.size() || i < velocities.size() || i < types.size() || i < lives.size() || i < textInfos.size() ||
                            i < gameOverInfos.size();
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
        fillEntityComponentData<ecs::component::GameOver>(reg, i, data, ecs::ComponentName::GAMEOVER);
        fillEntityComponentData<ecs::component::ActivePowerup>(reg, i, data, ecs::ComponentName::ACTIVEPOWERUP);
        fillEntityComponentData<ecs::component::MovementPattern>(reg, i, data, ecs::ComponentName::MOVEMENTPATTERN);
        fillEntityComponentData<ecs::component::Weapon>(reg, i, data, ecs::ComponentName::PLAYERWEAPON);
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
    componentCorrespondence.insert({ecs::ComponentName::GAMEOVER, ecs::component::GameOver{}});
    componentCorrespondence.insert({ecs::ComponentName::ACTIVEPOWERUP, ecs::component::ActivePowerup{}});
    componentCorrespondence.insert({ecs::ComponentName::MOVEMENTPATTERN, ecs::component::MovementPattern{}});
    componentCorrespondence.insert({ecs::ComponentName::PLAYERWEAPON, ecs::component::Weapon{}});
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
    insertComponent<ecs::component::GameOver>(reg, data, ecs::ComponentName::GAMEOVER);
    insertComponent<ecs::component::ActivePowerup>(reg, data, ecs::ComponentName::ACTIVEPOWERUP);
    insertComponent<ecs::component::MovementPattern>(reg, data, ecs::ComponentName::MOVEMENTPATTERN);
    insertComponent<ecs::component::Weapon>(reg, data, ecs::ComponentName::PLAYERWEAPON);
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
    dataToSend.deadEntities.erase(std::remove_if(dataToSend.deadEntities.begin(), dataToSend.deadEntities.end(),
                                                 [dataReceived](const auto &dataToSendEntity) {
                                                     return (std::find(dataReceived.deadEntities.begin(), dataReceived.deadEntities.end(),
                                                                       dataToSendEntity) != dataReceived.deadEntities.end());
                                                 }),
                                  dataToSend.deadEntities.end());

    dataToSend.entitiesData.erase(std::remove_if(dataToSend.entitiesData.begin(), dataToSend.entitiesData.end(),
                                                 [dataReceived](const auto &dataToSendEntity) {
                                                     return (std::find(dataReceived.entitiesData.begin(), dataReceived.entitiesData.end(),
                                                                       dataToSendEntity) != dataReceived.entitiesData.end());
                                                 }),
                                  dataToSend.entitiesData.end());
}
