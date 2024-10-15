/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** SpriteAnimationSystem
*/

#include "SpriteAnimationSystem.hpp"

void ecs::system::SpriteAnimationSystem::operator()(Registry &reg) {
    SparseArray<component::Drawable> &drawables = reg.getComponents<component::Drawable>();
    SparseArray<component::SpriteInfos> &spriteInfos = reg.getComponents<component::SpriteInfos>();
    static std::chrono::steady_clock::time_point time0 = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point time1 = std::chrono::steady_clock::now();
    std::chrono::milliseconds timeElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(time1 - time0);
    float offset = 0.0;
    float firstPatternLocation = 0.0;

    for (std::size_t i = 0; i < drawables.size() && i < spriteInfos.size(); ++i) {
        if (drawables[i] && spriteInfos[i]) {
            if (!drawables[i]->isLoaded)
                continue;
            if (timeElapsed.count() >= Lib::toMilliseconds(SPRITE_UPDATE_TIME)) {
                offset = float(drawables[i]->spriteTexture.width) / spriteInfos[i]->patternNb;
                firstPatternLocation = spriteInfos[i]->firstPattern * offset;
                if (drawables[i]->spriteRect.x + offset == float(drawables[i]->spriteTexture.width)) {
                    drawables[i]->spriteRect.x = firstPatternLocation;
                } else {
                    drawables[i]->spriteRect.x += offset;
                }
            }
        }
    }
    if (timeElapsed.count() >= Lib::toMilliseconds(SPRITE_UPDATE_TIME))
        time0 = std::chrono::steady_clock::now();
}
