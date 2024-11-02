/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** RaylibComponents
*/

/**
 * @file RaylibComponents.hpp
 * @brief Defines components used in the ECS system for the game engine.
 * This file contains the declaration of various client-side only components used in the ECS system,
 * because of the raylib dependencies of them. It contains components such as raylib objects for sound effects
 * or text font for example.
 */

#ifndef RAYLIBCOMPONENTS_HPP_
#define RAYLIBCOMPONENTS_HPP_

#define WIN32_LEAN_AND_MEAN
#define NOGDI


#include <windows.h>

#undef CloseWindow
#undef ShowCursor
#undef LoadImage
#undef DrawText
#undef DrawTextEx

#define CloseWindow CloseWindowWinAPI
#define ShowCursor ShowCursorWinAPI
#define LoadImage LoadImageWinAPI
#define DrawText DrawTextWinAPI
// #define DrawTextEx DrawTextExWinAPI

#include <raylib.h>

#undef CloseWindow
#undef ShowCursor
#undef LoadImage
#undef DrawText
// #undef DrawTextEx

#undef WIN32_LEAN_AND_MEAN
#undef NOGDI

namespace ecs {
    namespace component {
        /**
         * @struct Drawable
         * @brief Component representing the drawable texture of an entity.
         */
        struct Drawable {
            bool isLoaded = false;                 ///< Indicates if the texture is loaded
            Texture2D spriteTexture = Texture2D{}; ///< Texture for the entity
            Rectangle spriteRect = Rectangle{};    ///< Rectangle of the texture to be drawn
        };

        /**
         * @struct SoundEffectObject
         * @brief Component that store the sound effect object to play related to an entity.
         */
        struct SoundEffectObject {
            Sound sound = Sound{}; ///< Sound object
        };

        /**
         * @struct BackgroundMusicInfos
         * @brief Component that stores the background music object of an entity.
         */
        struct BackgroundMusicObject {
            Music music = Music{}; ///< Music object
        };

        /**
         * @struct TextFont
         * @brief Component for the text font linked to a text of an entity
         */
        struct TextFont {
            Font font = Font{}; ///< Font of the text
        };
    }
}

#endif /* !RAYLIBCOMPONENTS_HPP_ */
