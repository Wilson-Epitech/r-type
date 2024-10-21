/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** Registry
*/

/**
 * @file Registry.hpp
 * @brief Declares the `Registry` class for managing entities and components in the ECS.
 * This file contains the `Registry` class, which handles the creation, deletion, and management of entities
 * and components in the Entity Component System (ECS). It provides methods for spawning entities, registering components,
 * and running systems.
 */

#ifndef REGISTRY_HPP_
#define REGISTRY_HPP_

#include "Container.hpp"
#include "Error.hpp"
#include <any>
#include <functional>
#include <list>
#include <map>
#include <typeindex>
#include <unordered_map>

using entity_t = std::size_t;

namespace ecs {

    /**
     * @class Registry
     * @brief Manages entities, components, and systems in the ECS architecture.
     *
     * The `Registry` class is responsible for creating and deleting entities, managing components,
     * and executing systems in the game engine. It tracks entities and their associated components,
     * allowing systems to interact with them. The class also supports adding systems and initializing players.
     */
    class Registry {
      public:
        /**
         * @brief Creates a new entity and returns its identifier.
         * This method spawns a new entity, either by reusing a previously deleted entity or creating
         * a new one. The entity is added to the registry and assigned an identifier.
         * @return entity_t The identifier of the newly created entity.
         */
        entity_t spawnEntity() {
            entity_t entity;

            if (!_deadEntities.empty()) {
                entity = _deadEntities.front();
                _deadEntities.pop_front();
            } else {
                entity = _indexToEntities.size() + 1;
            }
            _indexToEntities[entity] = entity;
            _actualTurnActualTurnNewEntities.push_back(entity);
            return entity;
        };

        /**
         * @brief Deletes an entity and its associated components.
         * This method removes an entity from the registry, along with all of its associated components.
         * The entity is added to the list of dead entities to be recycled.
         * @param e The entity to be deleted.
         */
        void killEntity(entity_t const &e) {
            for (auto &it : _eraseEntity) {
                it.second(*this, e);
            }
            for (auto &it : _indexToEntities) {
                if (it.second == e) {
                    _deadEntities.push_back(std::size_t(it.second));
                    _actualTurnDeadEntities.push_back(std::size_t(it.second));
                    _indexToEntities.erase(it.first);
                    break;
                }
            }
            for (auto &player : _players) {
                if (player.second == e) {
                    _players.erase(player.first);
                    break;
                }
            }
        };

        /**
         * @brief Clears the list of new entities created in the current turn.
         */
        void clearActualTurnNewEntities() {
            _actualTurnActualTurnNewEntities.clear();
        }

        /**
         * @brief Returns the list of new entities created in the current turn.
         * @return std::list<std::size_t> A list of new entity identifiers created in the current turn.
         */
        std::list<std::size_t> getActualTurnNewEntities() {
            return _actualTurnActualTurnNewEntities;
        }

        /**
         * @brief Clears the list of dead entities from the current turn.
         */
        void clearActualTurnDeadEntities() {
            _actualTurnDeadEntities.clear();
        }

        /**
         * @brief Returns the list of entities deleted in the current turn.
         * @return std::list<std::size_t> A list of dead entity identifiers from the current turn.
         */
        std::list<std::size_t> getActualTurnDeadEntities() {
            return _actualTurnDeadEntities;
        }

        /**
         * @brief Retrieves the entity identifier from its index.
         * This method retrieves the entity corresponding to the given index.
         * @param idx The index of the entity.
         * @return entity_t The entity identifier.
         * @throws Error If no entity is found at the given index.
         */
        entity_t entityFromIndex(std::size_t idx) {
            auto it = _indexToEntities.find(idx);

            if (it == _indexToEntities.end())
                throw Error("No entity found at this index");
            return it->second;
        };

        /**
         * @brief Checks whether a given entity exists.
         * @param ent The entity to check.
         * @return true If the entity exists.
         * @return false If the entity does not exist.
         */
        bool entityExist(const entity_t &ent) {
            for (auto &it : _indexToEntities) {
                if (it.second == ent)
                    return true;
            }
            return false;
        }

        /**
         * @brief Adds a player entity associated with a client ID.
         * @param clientId The client ID.
         * @param entity The entity identifier of the player.
         * @throws Error If a player is already mapped to this client ID.
         */
        void addPlayer(std::size_t clientId, entity_t entity) {
            auto it = _players.find(clientId);
            if (it != _players.end())
                throw Error("Client already mapped with a player");
            _players[clientId] = entity;
        }

        /**
         * @brief Retrieves the player entity associated with a client ID.
         * @param clientId The client ID.
         * @return std::size_t The player entity identifier.
         * @throws Error If no player is mapped with this client ID.
         */
        std::size_t getPlayerByClientId(std::size_t clientId) {
            auto it = _players.find(clientId);
            if (it == _players.end())
                throw Error("No player mapped with this client id");
            return it->second;
        }

        /**
         * @brief Returns a map of all players and their associated client IDs.
         * @return std::map<std::size_t, entity_t> A map of client IDs to player entity identifiers.
         */
        std::map<std::size_t, entity_t> getPlayers() {
            return _players;
        }

        /**
         * @brief Removes a component from an entity.
         * @tparam Component The type of component to remove.
         * @param from The entity from which the component will be removed.
         */
        template <typename Component> void removeComponent(entity_t const &from) {
            SparseArray<Component> &component = getComponents<Component>();
            component.erase(from);
        };

        /**
         * @brief Registers a new component type in the ECS.
         * @tparam Component The type of component to register.
         * @return SparseArray<Component>& A reference to the newly registered component array.
         */
        template <class Component> SparseArray<Component> &registerComponent() {
            auto typeId = std::type_index(typeid(Component));
            auto it = _componentsArrays.find(typeId);

            if (it == _componentsArrays.end())
                _componentsArrays[typeId] = SparseArray<Component>();
            auto erase_func = [](Registry &reg, entity_t const &ent) {
                SparseArray<Component> &component = reg.getComponents<Component>();
                component.erase(ent);
            };
            _eraseEntity[typeId] = erase_func;

            return std::any_cast<SparseArray<Component> &>(_componentsArrays[typeId]);
        };

        /**
         * @brief Retrieves the array of components for a specific type.
         * @tparam Component The type of component to retrieve.
         * @return SparseArray<Component>& A reference to the array of components for the specified type.
         * @throws Error If the component type is not found.
         */
        template <class Component> SparseArray<Component> &getComponents() {
            auto typeId = std::type_index(typeid(Component));
            auto it = _componentsArrays.find(typeId);
            if (it == _componentsArrays.end())
                throw Error("Component not found");
            return std::any_cast<SparseArray<Component> &>(it->second);
        };

        template <class Component> SparseArray<Component> const &getComponents() const {
            auto typeId = std::type_index(typeid(Component));
            auto it = _componentsArrays.find(typeId);
            if (it == _componentsArrays.end())
                throw Error("Component not found");
            return std::any_cast<SparseArray<Component>>(it->second);
        };

        /**
         * @brief Adds a system to the ECS.
         * @tparam Components The component types used by the system.
         * @tparam Function The type of the function representing the system.
         * @param f The system function to add.
         */
        template <class... Components, typename Function> void addSystem(Function const &f) {
            _systems.push_back(f);
        };

        /**
         * @brief Executes all systems registered in the ECS.
         */
        void runSystems() {
            for (std::size_t i = 0; i < _systems.size(); i++) {
                _systems[i](*this);
            }
        };

        /**
         * @brief Adds a system for initializing player entities.
         * @tparam Components The component types used by the system.
         * @tparam Function The type of the function representing the system.
         * @param f The system function to add.
         */
        template <class... Components, typename Function> void addInitPlayerSystem(Function const &f) {
            _initPlayerSystem = f;
        };

        /**
         * @brief Executes the player initialization system.
         */
        void runInitPlayerSystem() {
            _initPlayerSystem(*this);
        };

        /**
         * @brief Adds a collision between two entities.
         * @param firstEntity The identifier of the first entity involved in the collision.
         * @param secondEntity The identifier of the second entity involved in the collision.
         */
        void addCollision(const std::size_t firstEntity, const std::size_t secondEntity) {
            _collisionEntity[firstEntity] = secondEntity;
        }

        /**
         * @brief Removes a collision entry for a specific entity.
         * @param index The identifier of the entity whose collision entry will be removed.
         */
        void eraseCollision(const std::size_t index) {
            _collisionEntity.erase(index);
        }

        /**
         * @brief Retrieves the map of all collisions between entities.
         * @return std::unordered_map<entity_t, entity_t> A map where the keys are entity identifiers and the values are the corresponding
         * entities they collided with.
         */
        std::unordered_map<entity_t, entity_t> getCollisionMap(void) {
            return _collisionEntity;
        }

      private:
        std::map<std::size_t, entity_t> _players;                        ///< A map of client IDs to player entity IDs.
        std::function<void(Registry &)> _initPlayerSystem;               ///< The player initialization system function.
        std::vector<std::function<void(Registry &)>> _systems;           ///< A list of systems to run each frame.
        std::list<std::size_t> _deadEntities;                            ///< A list of dead entities to be recycled.
        std::list<std::size_t> _actualTurnDeadEntities;                  ///< A list of entities that died this turn.
        std::list<std::size_t> _actualTurnActualTurnNewEntities;         ///< A list of new entities created this turn.
        std::map<std::size_t, entity_t> _indexToEntities;                ///< A map of entity indices to entity IDs.
        std::unordered_map<std::type_index, std::any> _componentsArrays; ///< A map of component arrays by type.
        std::unordered_map<std::type_index, std::function<void(Registry &, entity_t const &)>>
            _eraseEntity;                                        ///< Functions to remove components when an entity is deleted.
        std::unordered_map<entity_t, entity_t> _collisionEntity; ///< A map of entities indices who were in collision
    };
}

#endif
