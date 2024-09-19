/*
** R_TYPE PROJECT, 2024
** bs
** File description:
** Container
*/

#ifndef REGISTRY_HPP_
    #define REGISTRY_HPP_

    #include "Container.hpp"
    #include <unordered_map>
    #include <typeindex>
    #include <any>
    #include <string>
    #include <functional>

    using entity_t = std::size_t;

namespace RType {
    class Error : public std::exception
    {
        public:
            Error(const std::string& errorMessage):
            _errorMessage(errorMessage) {}
            const char *what() const noexcept
            {
                return this->_errorMessage.c_str();
            }
        private:
            std::string _errorMessage;
    };
}

class registry {
    public:

        entity_t spawn_entity() {
            entity_t entity = ++_alive_entities;

            if (!_dead_entities_index.empty()) {
                std::size_t index = _dead_entities_index.back();
                _dead_entities_index.pop_back();
                _index_to_entities[index] = entity;
            } else {
                _index_to_entities[_alive_entities] = entity;
            }
            return entity;
        }

        void kill_entity(entity_t const &e) {
            for (auto &it : _erase_entity) {
                it.second(*this, e);
            }
            for (auto &it : _index_to_entities) {
                if (it.second == e) {
                    _dead_entities_index.push_back(it.first);
                    it.second = 0;
                }
            }
            _alive_entities--;
        };

        entity_t entity_from_index(std::size_t idx) {
            auto it = _index_to_entities.find(idx);
            if (it == _index_to_entities.end())
                throw RType::Error("no entity found at this index");
            return it->second;
        }

        template <typename Component>
        void remove_component(entity_t const &from) {
            sparse_array<Component> &component = get_components<Component>();
            component.erase(from);
        }

        template <class Component>
        sparse_array<Component> &register_component() {
            auto typeId = std::type_index(typeid(Component));
            auto it = _components_arrays.find(typeId);
            if (it == _components_arrays.end())
                _components_arrays[typeId] = sparse_array<Component>();

                auto erase_func = [](registry &reg, entity_t const &ent) {
                    sparse_array<Component> &component = reg.get_components<Component>();
                    component.erase(ent);
                };
                _erase_entity[typeId] = erase_func;

            return std::any_cast<sparse_array<Component>&>(_components_arrays[typeId]);
        };

        template <class Component>
        sparse_array<Component> &get_components() {
            auto typeId = std::type_index(typeid(Component));
            auto it = _components_arrays.find(typeId);
            if (it == _components_arrays.end())
                throw RType::Error("Component not found");
            return std::any_cast<sparse_array<Component>&>(it->second);
        };

        template <class Component>
        sparse_array<Component> const &get_components() const {
            auto typeId = std::type_index(typeid(Component));
            auto it = _components_arrays.find(typeId);
            if (it == _components_arrays.end())
                throw RType::Error("Component not found");
            return std::any_cast<sparse_array<Component>>(it->second);
        };

    private:
        std::size_t _alive_entities = 0;
        std::vector<std::size_t> _dead_entities_index;
        std::unordered_map<std::type_index, std::any> _components_arrays;
        std::unordered_map<std::type_index, std::function<void(registry &, entity_t const &)>> _erase_entity;
        std::unordered_map<std::size_t, entity_t> _index_to_entities;
};

#endif