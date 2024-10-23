/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** DrawTextSystem
*/

#include "DrawTextSystem.hpp"

void ecs::system::DrawTextSystem::operator()(Registry &reg) {
    SparseArray<component::TextInfos> &textInfos = reg.getComponents<component::TextInfos>();
    SparseArray<component::TextFont> &textFonts = reg.getComponents<component::TextFont>();
    SparseArray<component::Type> &types = reg.getComponents<component::Type>();
    std::map<std::size_t, entity_t> player = reg.getPlayers();
    ecs::CustomVector2D screenRatio = Lib::getScreenRatio();

    if (player.size() != 1)
        return;
    entity_t playerEntity = player[0];
    loadFonts(textInfos, textFonts);
    for (std::size_t i = 0; i < textInfos.size() && i < types.size(); ++i) {
        if (textInfos[i] && textFonts[i] && types[i]) {
            if (types[i]->entityType == ecs::EntityType::PLAYER && i != playerEntity)
                continue;
            if (textInfos[i]->textToDisplay.length() > 0) {
                DrawTextEx(textFonts[i]->font, textInfos[i]->textToDisplay.c_str(), {(float)textInfos[i]->x / screenRatio.x, (float)textInfos[i]->y / screenRatio.y},
                           textInfos[i]->size / screenRatio.x, LETTER_SPACING, {textInfos[i]->color.r, textInfos[i]->color.g, textInfos[i]->color.b, textInfos[i]->color.a});
            }
        }
    }
}

void ecs::system::DrawTextSystem::loadFonts(SparseArray<component::TextInfos> &textInfos, SparseArray<component::TextFont> &textFonts) {
    for (std::size_t i = 0; i < textInfos.size(); ++i) {
        if (!textInfos[i])
            continue;
        if (!textFonts[i]) {
            textFonts.insertAt(i, ecs::component::TextFont{});
        }
        if (textFonts[i] && !textInfos[i]->fontIsLoaded && textInfos[i]->fontPath.length() > 0) {
            if (!FileExists(textInfos[i]->fontPath.c_str())) {
                textFonts[i]->font = GetFontDefault();
                textInfos[i]->fontIsLoaded = true;
            }
            textFonts[i]->font = LoadFont(textInfos[i]->fontPath.c_str());
            textInfos[i]->fontIsLoaded = true;
        }
    }
}
