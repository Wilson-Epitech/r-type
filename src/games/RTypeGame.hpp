/*
** EPrtypeTECH PROJECT, 2024
** r-type
** File description:
** RTypeGame
*/

#ifndef RTYPEGAME_HPP_
    #define RTYPEGAME_HPP_

    #include "AGame.hpp"
    #include "ControlSystem.hpp"
    #include "PositionSystem.hpp"
    #include "DrawSystem.hpp"

namespace game
{
    class RTypeGame : virtual public game::AGame
    {
        public:
            RTypeGame() : game::AGame() {
                initializeGame();
            };
            ~RTypeGame() = default;

            virtual void spawnEntities() override {
                _stringToEntity["player"] =  _reg.spawnEntity();
                _stringToEntity["movableEntity1"] =  _reg.spawnEntity();
                _stringToEntity["movableEntity2"] =  _reg.spawnEntity();
            };

            virtual void registerComponents() override {
                _reg.registerComponent<ecs::component::Position>();
                _reg.registerComponent<ecs::component::Velocity>();
                _reg.registerComponent<ecs::component::Drawable>();
                _reg.registerComponent<ecs::component::Controllable>();
            };

            virtual void addComponentsToEntities() override {
                ecs::SparseArray<ecs::component::Position> &positions = _reg.getComponents<ecs::component::Position>();
                ecs::SparseArray<ecs::component::Velocity> &velocities = _reg.getComponents<ecs::component::Velocity>();
                ecs::SparseArray<ecs::component::Drawable> &drawables = _reg.getComponents<ecs::component::Drawable>();
                ecs::SparseArray<ecs::component::Controllable> &controllables = _reg.getComponents<ecs::component::Controllable>();

                positions.insertAt(_stringToEntity["player"], ecs::component::Position{100, 100});
                velocities.insertAt(_stringToEntity["player"], ecs::component::Velocity{0, 0});
                controllables.insertAt(_stringToEntity["player"], ecs::component::Controllable{_key});
                drawables.insertAt(_stringToEntity["player"], ecs::component::Drawable{"../assets/r-typesheet39.gif"});

                positions.insertAt(_stringToEntity["movableEntity1"], ecs::component::Position{1500, 700});
                velocities.insertAt(_stringToEntity["movableEntity1"], ecs::component::Velocity{-0.4f, 0});
                drawables.insertAt(_stringToEntity["movableEntity1"], ecs::component::Drawable{"../assets/r-typesheet30a.gif"});

                positions.insertAt(_stringToEntity["movableEntity2"], ecs::component::Position{2000, 300});
                velocities.insertAt(_stringToEntity["movableEntity2"], ecs::component::Velocity{-0.3f, 0});
                drawables.insertAt(_stringToEntity["movableEntity2"], ecs::component::Drawable{"../assets/r-typesheet30a.gif"});
            };

            virtual void addSystemsToRegister() override {
                _reg.addSystem(ecs::system::ControlSystem());
                _reg.addSystem(ecs::system::PositionSystem());
                _reg.addSystem(ecs::system::DrawSystem());
            };

            ecs::Registry &getRegister() override {
                return _reg;
            }

            std::shared_ptr<KeyPressed> &getKey() {
                return _key;
            }
    };
}

#endif /* !RTYPEGAME_HPP_ */
