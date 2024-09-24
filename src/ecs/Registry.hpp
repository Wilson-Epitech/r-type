/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** Registry
*/

#ifndef REGISTRY_HPP_
    #define REGISTRY_HPP_

    #include "Error.hpp"
    #include "Container.hpp"
    #include <unordered_map>
    #include <functional>
    #include <typeindex>
    #include <list>
    #include <map>
    #include <any>

    using entity_t = std::size_t;

namespace ecs
{
    class Registry
    {
        public:

            entity_t spawnEntity() {
                entity_t entity = _indexToEntities.size() + 1;

                if (!_deadEntities.empty()) {
                    entity = _deadEntities.front();
                    _deadEntities.pop_front();
                    for (auto &it : _indexToEntities) {
                        if (it.second == 0) {
                            _indexToEntities[it.first] = entity;
                            break;
                        }
                    }
                } else {
                    _indexToEntities[entity] = entity;
                }
                return entity;
            };

            void killEntity(entity_t const &e) {
                for (auto &it : _eraseEntity) {
                    it.second(*this, e);
                }
                for (auto &it : _indexToEntities) {
                    if (it.second == e) {
                        _deadEntities.push_back(std::size_t(it.second));
                        it.second = 0;
                    }
                }
            };

            entity_t entityFromIndex(std::size_t idx) {
                auto it = _indexToEntities.find(idx);

                if (it == _indexToEntities.end())
                    throw Error("No entity found at this index");
                return it->second;
            };

            template <typename Component>
            void removeComponent(entity_t const &from) {
                SparseArray<Component> &component = getComponents<Component>();
                component.erase(from);
            };

            template <class Component>
            SparseArray<Component> &registerComponent() {
                auto typeId = std::type_index(typeid(Component));
                auto it = _componentsArrays.find(typeId);
                if (it == _componentsArrays.end())
                    _componentsArrays[typeId] = SparseArray<Component>();

                    auto erase_func = [](Registry &reg, entity_t const &ent) {
                        SparseArray<Component> &component = reg.getComponents<Component>();
                        component.erase(ent);
                    };
                    _eraseEntity[typeId] = erase_func;

                return std::any_cast<SparseArray<Component>&>(_componentsArrays[typeId]);
            };

            template <class Component>
            SparseArray<Component> &getComponents() {
                auto typeId = std::type_index(typeid(Component));
                auto it = _componentsArrays.find(typeId);
                if (it == _componentsArrays.end())
                    throw Error("Component not found");
                return std::any_cast<SparseArray<Component>&>(it->second);
            };

            template <class Component>
            SparseArray<Component> const &getComponents() const {
                auto typeId = std::type_index(typeid(Component));
                auto it = _componentsArrays.find(typeId);
                if (it == _componentsArrays.end())
                    throw Error("Component not found");
                return std::any_cast<SparseArray<Component>>(it->second);
            };

            template <class... Components, typename Function>

            void addSystem(Function const &f) {
                _systems.push_back(f);
            };

            void runSystems() {
                for (std::size_t i = 0; i < _systems.size(); i++) {
                    _systems[i](*this);
                }
            };

        private:
            std::size_t _totalEntities = 0;
            std::vector<std::function<void(Registry &)>> _systems;
            std::list<std::size_t> _deadEntities;
            std::map<std::size_t, entity_t> _indexToEntities;
            std::unordered_map<std::type_index, std::any> _componentsArrays;
            std::unordered_map<std::type_index, std::function<void(Registry &, entity_t const &)>> _eraseEntity;
    };
}

#endif