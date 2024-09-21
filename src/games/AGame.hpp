/*
** EPATECH PROJECT, 2024
** r-type
** File description:
** AGame
*/

#ifndef AGAME_HPP_
    #define AGAME_HPP_

    #include "Registry.hpp"
    #include "Components.hpp"
    #include "IGame.hpp"

namespace game
{
    class AGame : virtual public game::IGame
    {
        public:
            AGame() {
                _key = std::make_shared<KeyPressed>(KeyPressed::NO_KEY);
            }
            ~AGame() = default;

            virtual void initializeGame() override {
                spawnEntities();
                registerComponents();
                addComponentsToEntities();
                addSystemsToRegister();
            }

        protected:
            ecs::Registry _reg;
            std::unordered_map<std::string, entity_t> _stringToEntity;
            std::shared_ptr<KeyPressed> _key;
    };
}

#endif /* !AGAME_HPP_ */
