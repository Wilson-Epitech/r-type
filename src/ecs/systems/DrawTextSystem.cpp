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
    ecs::CustomVector2D screenRatio = {(float)Lib::getScreenWidth() / (float)GetScreenWidth(),
                                       (float)Lib::getScreenHeight() / (float)GetScreenHeight()};

    if (player.size() != 1)
        return;
    entity_t playerEntity = player[0];
    loadFontsTextInfos(textInfos, textFonts);
    for (std::size_t i = 0; i < textInfos.size() && i < types.size(); ++i) {
        if (textInfos[i] && textFonts[i] && types[i]) {
            if (types[i]->entityType == ecs::EntityType::PLAYER && i != playerEntity)
                continue;
            if (textInfos[i]->textToDisplay.length() > 0) {
                DrawTextEx(textFonts[i]->font, textInfos[i]->textToDisplay.c_str(),
                           {(float)textInfos[i]->x / screenRatio.x, (float)textInfos[i]->y / screenRatio.y},
                           textInfos[i]->size / screenRatio.x, LETTER_SPACING,
                           {textInfos[i]->color.r, textInfos[i]->color.g, textInfos[i]->color.b, textInfos[i]->color.a});
            }
        }
    }
    drawPowerupText(reg, playerEntity);
}

void ecs::system::DrawTextSystem::drawPowerupText(Registry &reg, const std::size_t &playerEntity) {
    SparseArray<component::ActivePowerup> &activePowerup = reg.getComponents<component::ActivePowerup>();
    SparseArray<component::TextInfos> &textInfos = reg.getComponents<component::TextInfos>();
    SparseArray<component::TextFont> &textFonts = reg.getComponents<component::TextFont>();
    ecs::CustomVector2D screenRatio = {(float)Lib::getScreenWidth() / (float)GetScreenWidth(),
        (float)Lib::getScreenHeight() / (float)GetScreenHeight()};
    std::size_t powerupTextInfosIndex = getPowerupTextInfos(reg);
    if (powerupTextInfosIndex == 0)
        return;
    std::string powerupString = "";

    loadFontsTextInfos(textInfos, textFonts);
    if (activePowerup.size() < playerEntity || textInfos.size() < powerupTextInfosIndex)
        return;
    if (!activePowerup[playerEntity] || !textInfos[powerupTextInfosIndex])
        return;
    if (activePowerup[playerEntity]->isInvincibleActive)
        powerupString += INVISIBLE_POWERUP_TEXT;
    if (activePowerup[playerEntity]->isMegaShootActive)
        powerupString += MEGASHOOT_POWERUP_TEXT;
    if (activePowerup[playerEntity]->isPowershootActive)
        powerupString += POWERSHOOT_POWERUP_TEXT;
    DrawTextEx(textFonts[powerupTextInfosIndex]->font, powerupString.c_str(), {powerupString.size() / screenRatio.x, (Lib::getScreenHeight() - textInfos[powerupTextInfosIndex]->size - LETTER_SPACING) / screenRatio.y},
            textInfos[powerupTextInfosIndex]->size / screenRatio.x, LETTER_SPACING, {textInfos[powerupTextInfosIndex]->color.r, textInfos[powerupTextInfosIndex]->color.g, textInfos[powerupTextInfosIndex]->color.b, textInfos[powerupTextInfosIndex]->color.a});
}

void ecs::system::DrawTextSystem::loadFontsTextInfos(SparseArray<component::TextInfos> &textInfos, SparseArray<component::TextFont> &textFonts) {
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

std::size_t ecs::system::DrawTextSystem::getPowerupTextInfos(Registry &reg) {
    SparseArray<component::TextInfos> &textInfos = reg.getComponents<component::TextInfos>();
    SparseArray<component::Type> &types = reg.getComponents<component::Type>();

    for (std::size_t i = 0; i < textInfos.size() && i < types.size(); ++i) {
        if (!textInfos[i] || !types[i])
            continue;
        if (types[i]->entityType == ecs::EntityType::POWERUP_TEMPLATE)
            return i;
    }
    return 0;
}
