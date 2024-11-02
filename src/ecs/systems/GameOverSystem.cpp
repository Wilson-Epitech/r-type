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
        if (gameOverComponents[i] && gameOverComponents[i]->isGameOver) {
            if (!gameOverComponents[i]->displayInitialized) {
                displayGameOverScreen(*gameOverComponents[i], reg);
                gameOverComponents[i]->displayInitialized = true;
            }
        }
    }
}

void ecs::system::GameOverSystem::displayGameOverScreen(const component::GameOver &gameOverComp, Registry &reg) {
    SparseArray<component::Type> &types = reg.getComponents<component::Type>();
    SparseArray<component::TextInfos> &textInfo = reg.getComponents<component::TextInfos>();

    for (std::size_t i = 0; i < types.size(); ++i) {
        if (types[i] && textInfo[i]) {
            reg.killEntity(i);
        }
    }
    int screenWidth = Lib::getScreenWidth();
    int screenHeight = Lib::getScreenHeight();

    entity_t gameOverTextEntity = reg.spawnEntity();
    textInfo.insertAt(gameOverTextEntity, component::TextInfos{GAME_OVER_TEXT, screenWidth / 12, screenHeight / 3, GAME_OVER_TEXT_SIZE,
                                                               FONT_PATH, RED_GAMEOVER});
    types.insertAt(gameOverTextEntity, component::Type{ecs::EntityType::TEXT});

    entity_t scoreTextEntity = reg.spawnEntity();
    std::string finalScoreText = FINAL_SCORE_TEXT + std::to_string(gameOverComp.finalScore);
    textInfo.insertAt(scoreTextEntity, component::TextInfos{finalScoreText, screenWidth / 12, screenHeight / 2, SCORE_TEXT_SIZE, FONT_PATH,
                                                            ORANGE_GAMEOVER});
    types.insertAt(scoreTextEntity, component::Type{ecs::EntityType::TEXT});

    entity_t exitTextEntity = reg.spawnEntity();
    textInfo.insertAt(exitTextEntity,
                      component::TextInfos{EXIT_PROMPT, static_cast<int>(screenWidth * 0.6), static_cast<int>(screenHeight * 0.85),
                                           EXIT_TEXT_SIZE, FONT_PATH, GREY_GAMEOVER});
    types.insertAt(exitTextEntity, component::Type{ecs::EntityType::TEXT});
}