/*
** R_TYPE PROJECT, 2024
** bs
** File description:
** entity
*/

#ifndef ENTITY_HPP_
    #define ENTITY_HPP_

    #include <iostream>

class Entity {
    public:
        Entity(std::size_t id) : _id(id) {};
        operator std::size_t() const {
            return this->_id;
        }
    private:
        std::size_t _id;
    };

#endif /* !ENTITY_HPP_ */
