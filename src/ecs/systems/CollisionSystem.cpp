/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** CollisionSystem
*/

#include "CollisionSystem.hpp"

void ecs::system::CollisionSystem::operator()(Registry &reg) {
    SparseArray<component::Position> &positions = reg.getComponents<component::Position>();
    SparseArray<component::Drawable> &drawables = reg.getComponents<component::Drawable>();
    SparseArray<component::Type> &types = reg.getComponents<component::Type>();
    SparseArray<component::Life> &lives = reg.getComponents<component::Life>();
    Rectangle rec1 = {0, 0, 0, 0};
    Rectangle rec2 = {0, 0, 0, 0};

    for (std::size_t i = 0; i < positions.size() && i < drawables.size() && i < types.size() && i < lives.size(); ++i) {
        if (!positions[i] || !drawables[i] || !types[i] || !lives[i])
            continue;
        if (lives[i]->nbLives == 0)
            continue;
        rec1 = spriteHitbox(positions, drawables, i);
        for (std::size_t j = i; j < positions.size() && j < drawables.size() && j < types.size() && j < lives.size(); ++j) {
            if (!positions[j] || !drawables[j] || !types[j] || !lives[j])
                continue;
            if (i == j || types[i]->type == types[j]->type || lives[j]->nbLives == 0)
                continue;
            rec2 = spriteHitbox(positions, drawables, j);
            if (CheckCollisionRecs(rec1, rec2) && (positions[i]->x > 0 && positions[i]->x < Lib::getScreenWidth()) &&
                !isPlayerAndPlayerShot(types[i]->type, types[j]->type)) {
                lives[i]->nbLives--;
                lives[j]->nbLives--;
                if (lives[i]->nbLives <= 0)
                    playSong(i, reg);
                if (lives[j]->nbLives <= 0)
                    playSong(j, reg);
            }
        }
    }
}

void ecs::system::CollisionSystem::playSong(std::size_t i, Registry &reg) {
    SparseArray<component::SoundEffect> &soundEffect = reg.getComponents<component::SoundEffect>();

    if (soundEffect[i])
        soundEffect[i]->asToBePlayed = true;
}

Rectangle ecs::system::CollisionSystem::spriteHitbox(SparseArray<component::Position> &positions,
                                                     SparseArray<component::Drawable> &drawables, std::size_t index) {
    return Rectangle{positions[index]->x, positions[index]->y, float(drawables[index]->spriteRect.width),
                     float(drawables[index]->spriteRect.height)};
}

bool ecs::system::CollisionSystem::isPlayerAndPlayerShot(ecs::EntityType &firstType, ecs::EntityType &secondType) {
    return ((firstType == ecs::EntityType::PLAYER && secondType == ecs::EntityType::PLAYERSHOT) ||
            (firstType == ecs::EntityType::PLAYERSHOT && secondType == ecs::EntityType::PLAYER));
}
