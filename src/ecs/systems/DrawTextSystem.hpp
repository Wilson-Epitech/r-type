/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** DrawTextSystem
*/

#ifndef DRAWTEXTSYSTEM_HPP_
#define DRAWTEXTSYSTEM_HPP_

#include "Components.hpp"
#include "ISystem.hpp"
#include "RaylibComponents.hpp"

#define LETTER_SPACING 2 ///< The spacing between two characters displayed

/** @brief Defines the text to display when the player is invinsible */
#define INVISIBLE_POWERUP_TEXT "Invincible|"
/** @brief Defines the text to display when the player has the powerup megaShoot enable*/
#define MEGASHOOT_POWERUP_TEXT "MegaShoot|"
/** @brief Defines the text to display when the player has the powerup powershoot enable */
#define POWERSHOOT_POWERUP_TEXT "Powershoot|"

namespace ecs {
    namespace system {

        /**
         * @class DrawTextSystem
         * @brief System responsible for rendering texts in the game.
         * The `DrawTextSystem` handles the rendering of texts linked to entities by drawing them on the screen.
         * It manages font loading and text changes.
         */
        class DrawTextSystem : virtual public ecs::system::ISystem {
          public:
            DrawTextSystem() = default;
            ~DrawTextSystem() = default;

            /**
             * @brief Executes the draw text system to render a text linked to an entity.
             * This method iterates over all entities and draws their corresponding text on the screen.
             * It handles the positioning, the scaling and the color.
             * @param reg The ECS registry containing entities and their components.
             */
            void operator()(Registry &reg) override;

          private:
            /**
             * @brief Load the font of a text.
             * @param textInfos The text informations of an entity.
             * @param textFonts The text fonts of an entity.
             */
            void loadFontsTextInfos(SparseArray<component::TextInfos> &textInfos, SparseArray<component::TextFont> &textFonts);

            /**
             * @brief Draw the text of the player active powerup.
             * @param reg The ECS registry containing entities and their components.
             * @param playerEntity The player entity.
             */
            void drawPowerupText(Registry &reg, const std::size_t &playerEntity);

            /**
             * @brief Draw the text of the player active powerup.
             * @param reg The ECS registry containing entities and their components.
             * @return std::size_t the index of the powerup template
             */
            std::size_t getPowerupTextInfos(Registry &reg);
        };
    }
}

#endif /* !DRAWTEXTSYSTEM_HPP_ */
