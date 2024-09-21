/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** LifeSystem
*/

#ifndef LIFESYSTEM_HPP_
    #define LIFESYSTEM_HPP_

    #include "ISystem.hpp"
    #include "Components.hpp"
    #include <raylib.h>

namespace ecs
{
    namespace system
    {
        class LifeSystem : virtual public ecs::system::ISystem
        {
            public:
                LifeSystem() = default;
                ~LifeSystem() = default;
                void operator()(Registry &reg) override
                {
                    SparseArray<component::Lives> &lives = reg.getComponents<component::Lives>();

                    for (std::size_t i = 0; i < lives.size(); ++i) {
                        if (lives[i]) {
                            if (lives[i]->nbLives == 0) {
                                reg.killEntity(reg.entityFromIndex(i));
                            }
                        }
                    }
                }
        };
    }
}

#endif /* !LIFESYSTEM_HPP_ */
