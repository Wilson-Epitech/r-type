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
    #include "CollisionSystem.hpp"
    #include "LifeSystem.hpp"
    #include "DrawSystem.hpp"
    #include "FiringSystem.hpp"

    #define PLAYER_NAME "player"
    #define FIRST_ENEMY_NAME "enemy1"
    #define SECOND_ENEMY_NAME "enemy2"
    #define THIRD_ENEMY_NAME "enemy3"

namespace game
{
    class RTypeGame : virtual public game::AGame
    {
        public:
            RTypeGame(const std::string gameName = "R-Type") : AGame(gameName) {
                initializeGame();
            };
            ~RTypeGame() = default;

            void spawnEntities() override {
                _stringToEntity[PLAYER_NAME] =  _reg.spawnEntity();
                _stringToEntity[FIRST_ENEMY_NAME] =  _reg.spawnEntity();
                _stringToEntity[SECOND_ENEMY_NAME] =  _reg.spawnEntity();
                _stringToEntity[THIRD_ENEMY_NAME] =  _reg.spawnEntity();
            };

            void registerComponents() override {
                _reg.registerComponent<ecs::component::Position>();
                _reg.registerComponent<ecs::component::Velocity>();
                _reg.registerComponent<ecs::component::Drawable>();
                _reg.registerComponent<ecs::component::Controllable>();
                _reg.registerComponent<ecs::component::Type>();
                _reg.registerComponent<ecs::component::Life>();
            };

            void addComponentsToEntities() override {
                ecs::SparseArray<ecs::component::Position> &positions = _reg.getComponents<ecs::component::Position>();
                ecs::SparseArray<ecs::component::Velocity> &velocities = _reg.getComponents<ecs::component::Velocity>();
                ecs::SparseArray<ecs::component::Drawable> &drawables = _reg.getComponents<ecs::component::Drawable>();
                ecs::SparseArray<ecs::component::Controllable> &controllables = _reg.getComponents<ecs::component::Controllable>();
                ecs::SparseArray<ecs::component::Type> &types = _reg.getComponents<ecs::component::Type>();
                ecs::SparseArray<ecs::component::Life> &lives = _reg.getComponents<ecs::component::Life>();

                positions.insertAt(_stringToEntity[PLAYER_NAME], ecs::component::Position{100, 100});
                velocities.insertAt(_stringToEntity[PLAYER_NAME], ecs::component::Velocity{0, 0});
                controllables.insertAt(_stringToEntity[PLAYER_NAME], ecs::component::Controllable{});
                drawables.insertAt(_stringToEntity[PLAYER_NAME], ecs::component::Drawable{TMP_PLAYER_PATH});
                types.insertAt(_stringToEntity[PLAYER_NAME], ecs::component::Type{ecs::entityType::PLAYER});
                lives.insertAt(_stringToEntity[PLAYER_NAME], ecs::component::Life{3});

                positions.insertAt(_stringToEntity[FIRST_ENEMY_NAME], ecs::component::Position{1500, 700});
                velocities.insertAt(_stringToEntity[FIRST_ENEMY_NAME], ecs::component::Velocity{-0.4f, 0});
                controllables.insertAt(_stringToEntity[FIRST_ENEMY_NAME], ecs::component::Controllable{false});
                drawables.insertAt(_stringToEntity[FIRST_ENEMY_NAME], ecs::component::Drawable{TMP_PLAYERSHOT_PATH});
                types.insertAt(_stringToEntity[FIRST_ENEMY_NAME], ecs::component::Type{ecs::entityType::ENEMY});
                lives.insertAt(_stringToEntity[FIRST_ENEMY_NAME], ecs::component::Life{1});

                positions.insertAt(_stringToEntity[SECOND_ENEMY_NAME], ecs::component::Position{1800, 300});
                velocities.insertAt(_stringToEntity[SECOND_ENEMY_NAME], ecs::component::Velocity{-0.3f, 0});
                drawables.insertAt(_stringToEntity[SECOND_ENEMY_NAME], ecs::component::Drawable{TMP_PLAYERSHOT_PATH});
                types.insertAt(_stringToEntity[SECOND_ENEMY_NAME], ecs::component::Type{ecs::entityType::ENEMY});
                lives.insertAt(_stringToEntity[SECOND_ENEMY_NAME], ecs::component::Life{1});

                positions.insertAt(_stringToEntity[THIRD_ENEMY_NAME], ecs::component::Position{1000, 300});
                velocities.insertAt(_stringToEntity[THIRD_ENEMY_NAME], ecs::component::Velocity{-0.3f, 0});
                drawables.insertAt(_stringToEntity[THIRD_ENEMY_NAME], ecs::component::Drawable{TMP_PLAYERSHOT_PATH});
                types.insertAt(_stringToEntity[THIRD_ENEMY_NAME], ecs::component::Type{ecs::entityType::ENEMY});
                lives.insertAt(_stringToEntity[THIRD_ENEMY_NAME], ecs::component::Life{1});
            };

            void addSystemsToRegister() override {
                _reg.addSystem(ecs::system::ControlSystem());
                _reg.addSystem(ecs::system::PositionSystem());
                _reg.addSystem(ecs::system::CollisionSystem());
                _reg.addSystem(ecs::system::LifeSystem());
                _reg.addSystem(ecs::system::FiringSystem());
                _reg.addSystem(ecs::system::DrawSystem());
            };

            ecs::Registry &getRegister() override {
                return _reg;
            };

            std::string &getGameName() override {
                return _gameName;
            };
    };
}

#endif /* !RTYPEGAME_HPP_ */
