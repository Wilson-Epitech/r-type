/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** Components
*/

#include "Components.hpp"
#include "Lib.hpp"

void operator<<(std::ostringstream &oss, const ecs::component::Position &position) {
    oss << position.x << ' ' << position.y;
}

void operator<<(std::ostringstream &oss, const ecs::component::Velocity &velocity) {
    oss << velocity.vx << ' ' << velocity.vy << ' ' << velocity.possibleVelocity;
}

void operator<<(std::ostringstream &oss, const ecs::component::SpriteInfos &spriteInfos) {
    oss << spriteInfos.patternNb << ' ' << spriteInfos.firstPattern << ' ' << spriteInfos.scaleFactor << ' ' << spriteInfos.spritePath
        << ' ' << spriteInfos.color.a << ' ' << spriteInfos.color.r << ' ' << spriteInfos.color.g << ' ' << spriteInfos.color.b;
}

void operator<<(std::ostringstream &oss, const ecs::component::Shooting &shooting) {
    oss << shooting.isShooting;
}

void operator<<(std::ostringstream &oss, const ecs::component::Type &type) {
    oss << type.entityType;
}

void operator<<(std::ostringstream &oss, const ecs::component::Life &life) {
    oss << life.nbLives << ' ' << life.displayLife << ' ' << life.maxLives;
}

void operator<<(std::ostringstream &oss, const ecs::component::SoundEffect &soundEffect) {
    oss << soundEffect.isLoaded << ' ' << soundEffect.hasToBePlayed << ' ' << soundEffect.soundPath;
}

void operator<<(std::ostringstream &oss, const ecs::component::BackgroundMusic &music) {
    oss << music.musicPath << ' ' << music.volume << ' ' << music.hasToBePlayed << ' ' << music.isLoaded << ' ' << music.isCurrentlyPlayed;
}

void operator<<(std::ostringstream &oss, const EntityData &entityData) {

    oss << entityData.entityID << ' ';
    oss << entityData.name << ' ';
    if (entityData.name == ecs::ComponentName::POSITION)
        Lib::serializeEntityCompotent<ecs::component::Position>(oss, entityData.name, entityData.component);
    if (entityData.name == ecs::ComponentName::VELOCITY)
        Lib::serializeEntityCompotent<ecs::component::Velocity>(oss, entityData.name, entityData.component);
    if (entityData.name == ecs::ComponentName::SPRITEINFOS)
        Lib::serializeEntityCompotent<ecs::component::SpriteInfos>(oss, entityData.name, entityData.component);
    if (entityData.name == ecs::ComponentName::SHOOTING)
        Lib::serializeEntityCompotent<ecs::component::Shooting>(oss, entityData.name, entityData.component);
    if (entityData.name == ecs::ComponentName::LIFE)
        Lib::serializeEntityCompotent<ecs::component::Life>(oss, entityData.name, entityData.component);
    if (entityData.name == ecs::ComponentName::TYPE)
        Lib::serializeEntityCompotent<ecs::component::Type>(oss, entityData.name, entityData.component);
    if (entityData.name == ecs::ComponentName::SOUNDEFFECT)
        Lib::serializeEntityCompotent<ecs::component::SoundEffect>(oss, entityData.name, entityData.component);
    if (entityData.name == ecs::ComponentName::MUSIC)
        Lib::serializeEntityCompotent<ecs::component::BackgroundMusic>(oss, entityData.name, entityData.component);
    oss << '\n';
}

void operator>>(std::istringstream &iss, ecs::EntityType &type) {
    int tmpType = 0;

    iss >> tmpType;
    type = static_cast<ecs::EntityType>(tmpType);
}

void operator>>(std::istringstream &iss, ecs::ComponentName &name) {
    int tmpName = 0;

    iss >> tmpName;
    name = static_cast<ecs::ComponentName>(tmpName);
}

void operator>>(std::istringstream &iss, ecs::component::Position &position) {
    iss >> position.x >> position.y;
}

void operator>>(std::istringstream &iss, ecs::component::Velocity &velocity) {
    iss >> velocity.vx >> velocity.vy >> velocity.possibleVelocity;
}

void operator>>(std::istringstream &iss, ecs::component::SpriteInfos &spriteInfos) {
    iss >> spriteInfos.patternNb >> spriteInfos.firstPattern >> spriteInfos.scaleFactor >> spriteInfos.spritePath >> spriteInfos.color.a >>
        spriteInfos.color.r >> spriteInfos.color.g >> spriteInfos.color.b;
}

void operator>>(std::istringstream &iss, ecs::component::Shooting &shooting) {
    iss >> shooting.isShooting;
}

void operator>>(std::istringstream &iss, ecs::component::Type &type) {
    iss >> type.entityType;
}

void operator>>(std::istringstream &iss, ecs::component::Life &life) {
    iss >> life.nbLives >> life.displayLife >> life.maxLives;
}

void operator>>(std::istringstream &iss, ecs::component::SoundEffect &soundEffect) {
    iss >> soundEffect.isLoaded >> soundEffect.hasToBePlayed >> soundEffect.soundPath;
}

void operator>>(std::istringstream &iss, ecs::component::BackgroundMusic &music) {
    iss >> music.musicPath >> music.volume >> music.hasToBePlayed >> music.isLoaded >> music.isCurrentlyPlayed;
}

void operator>>(std::istringstream &iss, EntityData &entityData) {

    iss >> entityData.entityID;
    iss >> entityData.name;
    if (entityData.name == ecs::ComponentName::POSITION)
        Lib::deserializeEntityCompotent<ecs::component::Position>(iss, entityData.name, entityData.component);
    if (entityData.name == ecs::ComponentName::VELOCITY)
        Lib::deserializeEntityCompotent<ecs::component::Velocity>(iss, entityData.name, entityData.component);
    if (entityData.name == ecs::ComponentName::SPRITEINFOS)
        Lib::deserializeEntityCompotent<ecs::component::SpriteInfos>(iss, entityData.name, entityData.component);
    if (entityData.name == ecs::ComponentName::SHOOTING)
        Lib::deserializeEntityCompotent<ecs::component::Shooting>(iss, entityData.name, entityData.component);
    if (entityData.name == ecs::ComponentName::LIFE)
        Lib::deserializeEntityCompotent<ecs::component::Life>(iss, entityData.name, entityData.component);
    if (entityData.name == ecs::ComponentName::TYPE)
        Lib::deserializeEntityCompotent<ecs::component::Type>(iss, entityData.name, entityData.component);
    if (entityData.name == ecs::ComponentName::SOUNDEFFECT)
        Lib::deserializeEntityCompotent<ecs::component::SoundEffect>(iss, entityData.name, entityData.component);
    if (entityData.name == ecs::ComponentName::MUSIC)
        Lib::deserializeEntityCompotent<ecs::component::BackgroundMusic>(iss, entityData.name, entityData.component);
}
