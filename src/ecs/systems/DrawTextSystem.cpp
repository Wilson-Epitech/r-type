/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** DrawTextSystem
*/

#include "DrawTextSystem.hpp"

void ecs::system::DrawTextSystem::operator()(Registry &reg) {
    SparseArray<component::TextInfos> &textInfos = reg.getComponents<component::TextInfos>();
    SparseArray<component::Type> &types = reg.getComponents<component::Type>();
    std::map<std::size_t, entity_t> player = reg.getPlayers();
    Vector2 screenRatio = Lib::getScreenRatio();
    if (player.size() != 1)
        return;
    entity_t playerEntity = player[0];

    loadFonts(textInfos);
    for (std::size_t i = 0; i < textInfos.size() && i < types.size(); ++i) {
        if (textInfos[i] && types[i]) {
            if (types[i]->entityType == ecs::EntityType::PLAYER && i != playerEntity)
                continue;
            if (textInfos[i]->textToDisplay.length() > 0) {
                DrawTextEx(textInfos[i]->font, textInfos[i]->textToDisplay.c_str(), {(float)textInfos[i]->x / screenRatio.x, (float)textInfos[i]->y / screenRatio.y},
                           textInfos[i]->size / screenRatio.x, LETTER_SPACING, textInfos[i]->color);
            }
        }
    }
}

void ecs::system::DrawTextSystem::loadFonts(SparseArray<component::TextInfos> &textInfos) {
    for (std::size_t i = 0; i < textInfos.size(); ++i) {
        if (!textInfos[i])
            continue;
        if (!textInfos[i]->fontIsLoaded && textInfos[i]->fontPath.length() > 0) {
            if (!FileExists(textInfos[i]->fontPath.c_str())) {
                textInfos[i]->font = GetFontDefault();
                textInfos[i]->fontIsLoaded = true;
            }
            textInfos[i]->font = LoadFont(textInfos[i]->fontPath.c_str());
            textInfos[i]->fontIsLoaded = true;
        }
    }
}
