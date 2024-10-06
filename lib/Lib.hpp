/*
** R_TYPE PROJECT, 2024
** responcive_window
** File description:
** lib
*/

#ifndef LIB_HPP_
#define LIB_HPP_

#include "Components.hpp"
#include "Registry.hpp"
#include <raylib.h>
#include <sstream>
#include <vector>

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

#define TO_MILLISECONDS 1000

class Lib {
  public:
    Lib() = default;
    ~Lib() = default;

    static Vector2 getScreenRatio();
    static int getScreenHeight();
    static int getScreenWidth();
    static bool isOnScreen(Vector2 pos);
    static double toMilliseconds(double time);
    static DataToSend fillNewEntitiesDataToSend(ecs::Registry &reg);
    static DataToSend fillAllEntitiesDataToSend(ecs::Registry &reg);
    static std::string serializeEntityDataVector(const DataToSend &data);
    static DataToSend deserializeEntityDataVector(const std::string &message);
    static void updateEntities(ecs::Registry &reg, EntityData &data);

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
    static std::vector<EntityData> retrievePlayersVelocitiesUpdates(ecs::Registry &reg);
    static std::vector<EntityData> retrievePlayersShootingsUpdates(ecs::Registry &reg);
    static std::vector<std::size_t> retrievePlayersIndex(ecs::Registry &reg);
    static std::vector<std::pair<ecs::ComponentName, std::any>> getComponentCorrespondance();

    template <typename Component>
    static void fillEntityComponentData(ecs::Registry &reg, const std::size_t &newEntity, DataToSend &data, const ecs::ComponentName name) {
        auto &components = reg.getComponents<Component>();

        if (components[newEntity]) {
            EntityData entityData;
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

#endif /* !LIB_HPP_ */
