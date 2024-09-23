/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** IGame
*/

#ifndef IGAME_HPP_
    #define IGAME_HPP_

namespace game
{
    class IGame
    {
        public:
            IGame() = default;
            ~IGame() = default;

            virtual void initializeGame() = 0;
            virtual void spawnEntities() = 0;
            virtual void registerComponents() = 0;
            virtual void addComponentsToEntities() = 0;
            virtual void addSystemsToRegister() = 0;
            virtual ecs::Registry &getRegister() = 0;
            virtual std::string &getGameName() = 0;
    };
}

#endif /* !IGAME_HPP_ */
