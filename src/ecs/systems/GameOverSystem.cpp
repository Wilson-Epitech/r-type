/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** FiringSystem
*/

#include "GameOverSystem.hpp"
#include <iostream>

void ecs::system::GameOverSystem::operator()(Registry &reg) {
    SparseArray<component::GameOver> &gameOverComponents = reg.getComponents<component::GameOver>();

    for (std::size_t i = 0; i < gameOverComponents.size(); ++i) {
        if (gameOverComponents[i]) {
            if (gameOverComponents[i]->isGameOver && !gameOverComponents[i]->displayInitialized) {
                displayGameOverScreen(reg);
                gameOverComponents[i]->displayInitialized = true;
            }
        }
    }
}

void ecs::system::GameOverSystem::displayGameOverScreen(Registry &reg) {
    SparseArray<component::Type> &types = reg.getComponents<component::Type>();
    SparseArray<component::TextInfos> &textInfo = reg.getComponents<component::TextInfos>();
    SparseArray<component::Score> &scoreinfo = reg.getComponents<ecs::component::Score>();
    int score = 0;

    for (std::size_t i = 0; i < types.size(); ++i) {
        if (scoreinfo[i] && types[i]) {
            if (types[i]->entityType != ecs::EntityType::PLAYER)
                score = scoreinfo[i]->score;
        }
        if (types[i] && textInfo[i])
            reg.killEntity(i);
    }
    int screenWidth = Lib::getScreenWidth();
    int screenHeight = Lib::getScreenHeight();

    entity_t gameOverTextEntity = reg.spawnEntity();
    textInfo.insertAt(gameOverTextEntity,
                      component::TextInfos{GAME_OVER_TEXT, screenWidth / WIDTH_GAME_OVER, screenHeight / HEIGHT_GAME_OVER,
                                           GAME_OVER_TEXT_SIZE, FONT_PATH, RED_GAMEOVER});
    types.insertAt(gameOverTextEntity, component::Type{ecs::EntityType::TEXT});

    entity_t scoreTextEntity = reg.spawnEntity();
    std::string finalScoreText = FINAL_SCORE_TEXT + std::to_string(score);
    textInfo.insertAt(scoreTextEntity, component::TextInfos{finalScoreText, screenWidth / WIDTH_GAME_OVER, screenHeight / HEIGHT_SCORE,
                                                            SCORE_TEXT_SIZE, FONT_PATH, ORANGE_GAMEOVER});
    types.insertAt(scoreTextEntity, component::Type{ecs::EntityType::TEXT});

    entity_t exitTextEntity = reg.spawnEntity();
    textInfo.insertAt(exitTextEntity,
                      component::TextInfos{EXIT_PROMPT, static_cast<int>(screenWidth * WIDTH_EXIT_TEXT),
                                           static_cast<int>(screenHeight * HEIGHT_EXIT_TEXT), EXIT_TEXT_SIZE, FONT_PATH, GREY_GAMEOVER});
    types.insertAt(exitTextEntity, component::Type{ecs::EntityType::TEXT});
}