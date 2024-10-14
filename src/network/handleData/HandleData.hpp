/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** HandleData
*/

#ifndef HANDLE_DATA_HPP_
#define HANDLE_DATA_HPP_

#include "Components.hpp"
#include "Registry.hpp"

namespace network {
class HandleData {
  public:
    HandleData() = default;
    ~HandleData() = default;

    static DataToSend fillNewEntitiesDataToSend(ecs::Registry &reg);
    static DataToSend fillAllEntitiesDataToSend(ecs::Registry &reg);
    static std::string serializeEntityDataVector(const DataToSend &data);
    static DataToSend deserializeEntityDataVector(const std::string &message);
    static void updateEntities(ecs::Registry &reg, EntityData &data);
    static void deleteSimilarDataLines(DataToSend &dataToSend, const DataToSend &dataReceived);

    template <typename Component>
    static void serializeEntityCompotent(std::ostringstream &oss, const ecs::ComponentName &entityComponentName,
                                         const std::any &component) {
        std::vector<std::pair<ecs::ComponentName, std::any>> componentCorrespondance = getComponentCorrespondance();
        for (auto &it : componentCorrespondance) {
            if (entityComponentName == it.first) {
                auto tmpComponent = std::any_cast<Component>(component);
                oss << tmpComponent;
                return;
            }
        }
    }

    template <typename Component>
    static void deserializeEntityCompotent(std::istringstream &iss, const ecs::ComponentName &entityComponentName, std::any &component) {
        std::vector<std::pair<ecs::ComponentName, std::any>> componentCorrespondance = getComponentCorrespondance();
        for (auto &it : componentCorrespondance) {
            if (entityComponentName == it.first) {
                auto tmpComponent = std::any_cast<Component>(it.second);
                iss >> tmpComponent;
                component = tmpComponent;
                return;
            }
        }
    }

  private:
    static std::vector<std::pair<ecs::ComponentName, std::any>> getComponentCorrespondance();
    static std::vector<std::size_t> retrievePlayersIndex(ecs::Registry &reg);

    template <typename Component>
    static std::vector<EntityData> retrievePlayersComponentUpdates(ecs::Registry &reg, ecs::ComponentName name) {
        static std::vector<typename ecs::SparseArray<Component>::value_type> lastPlayersValues;
        ecs::SparseArray<Component> &shootings = reg.getComponents<Component>();
        std::vector<std::size_t> playersIndex = retrievePlayersIndex(reg);
        std::vector<EntityData> playersData;
        EntityData playerData = EntityData{};
        std::size_t turn = 0;

        for (auto &playerIndex : playersIndex) {
            if (playerIndex == 0 || !shootings[playerIndex]) {
                continue;
            }
            if (lastPlayersValues.size() >= turn + 1) {
                if (lastPlayersValues[turn] != shootings[playerIndex].value()) {
                    playerData.entityID = playerIndex;
                    playerData.component = shootings[playerIndex].value();
                    playerData.name = name;
                    playersData.push_back(playerData);
                    lastPlayersValues[turn] = shootings[playerIndex].value();
                }
            } else {
                playerData.entityID = playerIndex;
                playerData.component = shootings[playerIndex].value();
                playerData.name = name;
                playersData.push_back(playerData);
                lastPlayersValues.push_back(shootings[playerIndex].value());
            }
            turn++;
        }
        return playersData;
    }

    template <typename Component>
    static void fillEntityComponentData(ecs::Registry &reg, const std::size_t &newEntity, DataToSend &data, const ecs::ComponentName name) {
        auto &components = reg.getComponents<Component>();

        if (components[newEntity]) {
            EntityData entityData = EntityData{};
            entityData.entityID = newEntity;
            entityData.component = components[newEntity].value();
            entityData.name = name;
            data.entitiesData.push_back(entityData);
        }
    }
    template <typename Component>
    static void insertComponent(ecs::Registry &reg, const EntityData &entityData, const ecs::ComponentName name) {
        if (entityData.name == name) {
            auto &components = reg.getComponents<Component>();
            auto component = std::any_cast<Component>(entityData.component);
            components.insertAt(entityData.entityID, component);
        }
    }
};
};

#endif /* !HANDLE_DATA_HPP_ */
