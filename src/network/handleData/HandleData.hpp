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
    /**
     * @class HandleData.hpp
     * @brief Class providing static functions for managing and retrieving game data.
     * Contains helper methods for data serialization/deserialization, entity data and component manipulation.
     */
    class HandleData {
      public:
        HandleData() = default;
        ~HandleData() = default;

        /**
         * @brief Fills a DataToSend object with data for newly spawned entities in the ECS.
         * @param reg The ECS registry containing the components and entities.
         * @return A DataToSend structure containing data for the newly spawned entities.
         */
        static DataToSend fillNewEntitiesDataToSend(ecs::Registry &reg);

        /**
         * @brief Fills a DataToSend object with data for all entities in the ECS.
         * @param reg The ECS registry containing the components and entities.
         * @return A DataToSend structure containing data for all entities in the registry.
         */
        static DataToSend fillAllEntitiesDataToSend(ecs::Registry &reg);
        /**
         * @brief Serializes a DataToSend structure into a string format.
         * @param data The data to serialize.
         * @return A string representation of the serialized data.
         */
        static std::string serializeEntityDataVector(const DataToSend &data);

        /**
         * @brief Deserializes a string into a DataToSend structure.
         * @param message The serialized string data.
         * @return A DataToSend structure containing the deserialized entity data.
         */
        static DataToSend deserializeEntityDataVector(const std::string &message);

        /**
         * @brief Updates the EntityData of the entity in the ECS registry based on the received entity.
         * @param reg The ECS registry.
         * @param data The entity data to update.
         */
        static void updateEntity(ecs::Registry &reg, EntityData &data);

        /**
         * @brief Kill entities from the registry based on the entities present in the deadEntities list from DataToSend struct.
         * @param reg The ECS registry.
         * @param data The DataToSend struct.
         */
        static void updateDeadEntities(DataToSend &dataReceived, ecs::Registry &reg);
        /**
         * @brief Updates the EntityData of entities in the ECS registry based on the entities present in the vector of entitiesData from DataToSend struct.
         * @param reg The ECS registry.
         * @param data The DataToSend struct.
         */
        static void updateEntitiesData(DataToSend &dataReceived, ecs::Registry &reg);
        /**
         * @brief Deletes similar lines of data between two DataToSend objects.
         * This function compares two DataToSend objects and removes matching lines from the first.
         * @param dataToSend The DataToSend structure to modify.
         * @param dataReceived The DataToSend structure to compare against.
         */
        static void deleteSimilarDataLines(DataToSend &dataToSend, const DataToSend &dataReceived);

        /**
         * @brief Serializes a specific component type from an entity.
         * @tparam Component The component type to serialize.
         * @param oss The output string stream to store the serialized data.
         * @param entityComponentName The name of the component.
         * @param component The component data to serialize.
         */
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

        /**
         * @brief Deserializes a specific component type into an entity.
         * @tparam Component The component type to deserialize.
         * @param iss The input string stream containing the serialized data.
         * @param entityComponentName The name of the component.
         * @param component The component to store the deserialized data.
         */
        template <typename Component>
        static void deserializeEntityCompotent(std::istringstream &iss, const ecs::ComponentName &entityComponentName,
                                               std::any &component) {
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
        /**
         * @brief Retrieves the list of component types and their corresponding default values.
         * @return A vector of pairs containing the component names and their default values.
         */
        static std::vector<std::pair<ecs::ComponentName, std::any>> getComponentCorrespondance();
        /**
         * @brief Retrieves the indices of player entities in the ECS registry.
         * @param reg The ECS registry.
         * @return A vector of player entity indices.
         */
        static std::vector<std::size_t> retrievePlayersIndex(ecs::Registry &reg);

        /**
         * @brief Retrieves updates for a specific component type for all players in the ECS registry.
         * @tparam Component The component type to retrieve updates for.
         * @param reg The ECS registry.
         * @param name The name of the component.
         * @return A vector of EntityData containing updated component data for the players.
         */
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

        /**
         * @brief Fills a DataToSend structure with component data for a specific entity.
         * @tparam Component The component type to retrieve data from.
         * @param reg The ECS registry.
         * @param newEntity The entity ID to retrieve data for.
         * @param data The DataToSend structure to populate.
         * @param name The name of the component.
         */
        template <typename Component>
        static void fillEntityComponentData(ecs::Registry &reg, const std::size_t &newEntity, DataToSend &data,
                                            const ecs::ComponentName name) {
            auto &components = reg.getComponents<Component>();

            if (components[newEntity]) {
                EntityData entityData = EntityData{};
                entityData.entityID = newEntity;
                entityData.component = components[newEntity].value();
                entityData.name = name;
                data.entitiesData.push_back(entityData);
            }
        }

        /**
         * @brief Inserts a component into the ECS registry for a specific entity.
         * @tparam Component The component type to insert.
         * @param reg The ECS registry.
         * @param entityData The entity data containing the component to insert.
         * @param name The name of the component.
         */
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
