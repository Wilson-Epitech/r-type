/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** Assets
*/

/**
 * @file Assets.hpp
 * @brief Contains all the path necessary for the RType game, like sprites, sounds, and fonts.
 * It provides also special informations for sprites like their pattern number, their first pattern to display and their scale factor.
 */

#ifndef ASSETS_HPP_
#define ASSETS_HPP_

#define PLAYER_PATH "assets/r-typesheet5.png"                 ///< The player sprite path.
#define PLAYERSHOT_PATH "assets/r-typesheet30a.png"           ///< The playershot sprite path.
#define ENEMY_PATH "assets/r-typesheet17.png"                 ///< The enemy sprite path.
#define LEVEL2_ENEMY_PATH "assets/level2_ennemies.png"        ///< The level 2 enemy sprite path.
#define ENEMYSHOT_PATH "assets/enemy_bullet.png"              ///< The enemyshot sprite path.
#define LEVEL1_BOSS_PATH "assets/r-typesheet41.png"           ///< The level 1 boss sprite path.
#define STRONGER_ENEMY_PATH "assets/small_boss.png"           ///< The stronger enemy sprite path.
#define LEVEL1_BOSS_SHOT_PATH "assets/boss1_shoot.png"        ///< The level 1 boss shot path.
#define LEVEL1_BACKGROUND_PATH "assets/background.png"        ///< The level 1 background sprite path.
#define LEVEL2_BACKGROUND_PATH "assets/background_level2.png" ///< The level 2 background sprite path.
#define POWERUP_PATH "assets/powerup.png"                     ///< The powerup sprite path.
#define SHOOT_SOUND_PATH "assets/shoot.wav"                   ///< The shoot sound path.
#define EXPLOSION_SOUND_PATH "assets/explosion.wav"           ///< The explosion sound path.
#define BACKGROUND_MUSIC_PATH "assets/backgroundMusic.mp3"    ///< The background music path.
#define FONT_PATH "assets/ARCADE_N.TTF"                       ///< The text font path.
#define WEAPON_PATH "assets/additionalWeapon.png"

#define PLAYER_SPRITE_NB_PATTERN 16        ///< The player sprite pattern number.
#define PLAYERSHOT_SPRITE_NB_PATTERN 3     ///< The playershot sprite pattern number.
#define ENEMY_SPRITE_NB_PATTERN 8          ///< The enemy sprite pattern number.
#define ENEMYSHOT_SPRITE_NB_PATTERN 2      ///< The enemyshot sprite pattern number.
#define STRONGER_ENEMY_SPRITE_NB_PATTERN 4 ///< The stronger enemy sprites pattern number.
#define LEVEL1_BOSS_NB_PATTERN 5           ///< The level 1 boss sprite pattern number.
#define LEVEL1_BOSS_SHOT_PATTERN 4         ///< The level 1 boss shot sprite pattern number.
#define BACKGROUND_SPRITE_NB_PATTERN 1     ///< The background sprite pattern number.
#define POWERUP_SPRITE_NB_PATTERN 10       ///< The powerup sprite pattern number.

#define PLAYER_SPRITE_FIRST_PATTERN 8  ///< The first pattern to display for the player sprite.
#define POWERUP_SPRITE_FIRST_PATTERN 1 ///< The first pattern to display for the powerup sprite.

#define PLAYER_SPRITE_SCALE_FACTOR 2.3 ///< The scale factor for the player sprite.
#define ENEMY_SCALE_FACTOR 1           ///< The scale factor for the enemy sprite.
#define ENEMYSHOT_SCALE_FACTOR 2       ///< The scale factor for the enemyshot sprite.
#define STRONGER_ENEMY_SCALE_FACTOR 5  ///< The scale factor for the stronger enemy sprite.
#define LEVEL1_BOSS_SCALE_FACTOR 10    ///< The scale factor for the level 1 boss sprite.
#define BACKGROUND_SCALE_FACTOR 0.625  ///< The scale factor for the background sprite.
#define POWERUP_SCALE_FACTOR 0.5       ///< The scale factor for the powerup sprite.

#endif /* !ASSETS_HPP_ */