/*
** R_TYPE PROJECT, 2024
** branch
** File description:
** ScoreSystem
*/

#include "ScoreSystem.hpp"

void ecs::system::ScoreSystem::operator()(Registry &reg) {
    SparseArray<component::Type> &types = reg.getComponents<component::Type>();
    SparseArray<component::Score> &scores = reg.getComponents<component::Score>();
    SparseArray<component::ShootPlayerIndex> &shootPlayerIndex = reg.getComponents<component::ShootPlayerIndex>();
    std::unordered_map<entity_t, entity_t> collisionMap = reg.getCollisionMap();
    entity_t playerIndex = 0;
    entity_t teamScoreIndex = getScoreTextIndex(reg);

    for (std::size_t i = 0; i < types.size(); ++i) {
        if (collisionMap.find(i) == collisionMap.end())
            continue;
        if (!types[i]) {
            reg.eraseCollision(i);
            continue;
        }
        if (types[i]->entityType == ecs::EntityType::PLAYERSHOT) {
            if (shootPlayerIndex[i]) {
                playerIndex = shootPlayerIndex[i]->playerIndex;
            }
        } else if (types[collisionMap[i]]->entityType == ecs::EntityType::PLAYERSHOT) {
            if (shootPlayerIndex[collisionMap[i]]) {
                playerIndex = shootPlayerIndex[collisionMap[i]]->playerIndex;
            }
        }
        if ((types[i]->entityType == ecs::EntityType::ENEMY || types[collisionMap[i]]->entityType == ecs::EntityType::ENEMY) &&
            playerIndex != 0) {
            scores[playerIndex]->score += scores[playerIndex]->scoreIncrementation;
            scores[teamScoreIndex]->score += scores[playerIndex]->scoreIncrementation;
            fillTextInfos(playerIndex, reg);
            fillTextInfos(teamScoreIndex, reg);
        }
        reg.eraseCollision(i);
    }
}

void ecs::system::ScoreSystem::fillTextInfos(const std::size_t index, Registry &reg) {
    SparseArray<component::Score> &score = reg.getComponents<component::Score>();
    SparseArray<component::TextInfos> &textInfos = reg.getComponents<component::TextInfos>();

    if (score.size() < index || textInfos.size() < index)
        return;
    if (!score[index] || !textInfos[index])
        return;
    textInfos[index]->textToDisplay = removeNumber(textInfos[index]->textToDisplay) + std::to_string(score[index]->score);
    textInfos[index]->hasChanged = true;
}

std::string ecs::system::ScoreSystem::removeNumber(std::string str) {
    for (std::size_t i = str.length(); i > 0; i--) {
        if (str[i] <= '9' && str[i] >= '0') {
            str.pop_back();
        }
    }
    return str;
}

std::size_t ecs::system::ScoreSystem::getScoreTextIndex(Registry &reg) {
    SparseArray<component::Type> &types = reg.getComponents<component::Type>();
    SparseArray<component::Score> &scores = reg.getComponents<component::Score>();
    SparseArray<component::TextInfos> &textInfos = reg.getComponents<component::TextInfos>();

    for (std::size_t i = 0; i < types.size() && i < scores.size() && i < textInfos.size(); ++i) {
        if (!types[i] || !scores[i] || !textInfos[i])
            continue;
        if (types[i]->entityType == ecs::EntityType::TEXT)
            return i;
    }
    return 0;
}