/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** DrawSystem
*/

#ifndef DRAWSYSTEM_HPP_
    #define DRAWSYSTEM_HPP_

    #include "ISystem.hpp"
    #include "Components.hpp"

namespace rtype
{
    class DrawSystem : virtual public rtype::ISystem
    {
        public:
            DrawSystem() = default;
            ~DrawSystem() = default;
            void operator()(registry &reg) override
            {
                sparse_array<component::Position> &positions = reg.get_components<component::Position>();
                sparse_array<component::Drawable> &drawables = reg.get_components<component::Drawable>();

                load_all_textures(drawables);
                for (std::size_t i = 0; i < drawables.size() && i < positions.size(); ++i) {
                    if (drawables[i] && positions[i]) {
                        if (drawables[i]->sprite_path.length() > 0) {
                            DrawTexture(drawables[i]->sprite_texture, positions[i]->x, positions[i]->y, RAYWHITE);
                        }
                    }
                }
            };

            void load_all_textures(sparse_array<component::Drawable> &drawables)
            {
                static bool is_loaded = false;

                if (is_loaded)
                    return;
                for (std::size_t i = 0; i < drawables.size(); ++i) {
                    if (drawables[i]) {
                        if (drawables[i]->sprite_path.length() > 0) {
                            drawables[i]->sprite_texture = LoadTexture(drawables[i]->sprite_path.c_str());
                        }
                    }
                }
                is_loaded = true;
            }
    };
}

#endif /* !DRAWSYSTEM_HPP_ */
