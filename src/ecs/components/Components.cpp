/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** Components
*/

#include "Components.hpp"
#include "HandleData.hpp"

void operator<<(std::ostringstream &oss, const ecs::component::Position &position) {
    oss << position.x << ' ' << position.y << ' ' << position.startX << ' ' << position.startY;
}

void operator<<(std::ostringstream &oss, const ecs::component::Velocity &velocity) {
    oss << velocity.vx << ' ' << velocity.vy << ' ' << velocity.possibleVelocity;
}

void operator<<(std::ostringstream &oss, const ecs::component::SpriteInfos &spriteInfos) {
    oss << spriteInfos.patternNb << ' ' << spriteInfos.firstPattern << ' ' << spriteInfos.scaleFactor << ' ' << spriteInfos.spritePath
        << ' ' << spriteInfos.color.a << ' ' << spriteInfos.color.r << ' ' << spriteInfos.color.g << ' ' << spriteInfos.color.b << ' '
        << spriteInfos.spriteWidth << ' ' << spriteInfos.spriteHeight << ' ' << spriteInfos.rotationAngle;
}

void operator<<(std::ostringstream &oss, const ecs::component::Shooting &shooting) {
    oss << shooting.isShooting << ' ' << shooting.shootPower << ' ' << shooting.shootSize;
}

void operator<<(std::ostringstream &oss, const ecs::component::Type &type) {
    oss << type.entityType;
}

void operator<<(std::ostringstream &oss, const ecs::component::Life &life) {
    oss << life.nbLives << ' ' << life.displayLife << ' ' << life.maxLives << ' ' << life.isInvincible;
}

void operator<<(std::ostringstream &oss, const ecs::component::SoundEffectInfos &soundEffectInfos) {
    oss << soundEffectInfos.isLoaded << ' ' << soundEffectInfos.hasToBePlayed << ' ' << soundEffectInfos.soundPath;
}

void operator<<(std::ostringstream &oss, const ecs::component::BackgroundMusicInfos &music) {
    oss << music.musicPath << ' ' << music.volume << ' ' << music.hasToBePlayed << ' ' << music.isLoaded << ' ' << music.isCurrentlyPlayed;
}

void operator<<(std::ostringstream &oss, const ecs::component::Score &score) {
    oss << score.score << ' ' << score.scoreIncrementation;
}

void operator<<(std::ostringstream &oss, const ecs::component::TextInfos &textInfo) {
    oss << textInfo.textToDisplay << ' ' << textInfo.x << ' ' << textInfo.y << ' ' << textInfo.size << ' ' << textInfo.fontPath << ' '
        << textInfo.color.a << ' ' << textInfo.color.r << ' ' << textInfo.color.g << ' ' << textInfo.color.b;
}

void operator<<(std::ostringstream &oss, const ecs::component::ActivePowerup &activePowerup) {
    oss << activePowerup.isPowershootActive << ' ' << activePowerup.isInvincibleActive << ' ' << activePowerup.isMegaShootActive;
}

void operator<<(std::ostringstream &oss, const ecs::component::MovementPattern &movementPattern) {
    oss << movementPattern.patternAmplitude << ' ' << movementPattern.isReversible << ' ' << movementPattern.movementPatternType;
}

void operator<<(std::ostringstream &oss, const ecs::component::Weapon &weapons) {
    oss << weapons.weaponType << ' ' << weapons.playerIndex;
}

void operator<<(std::ostringstream &oss, const EntityData &entityData) {

    oss << entityData.entityID << ' ';
    oss << entityData.name << ' ';
    if (entityData.name == ecs::ComponentName::POSITION)
        network::HandleData::serializeEntityCompotent<ecs::component::Position>(oss, entityData.name, entityData.component);
    if (entityData.name == ecs::ComponentName::VELOCITY)
        network::HandleData::serializeEntityCompotent<ecs::component::Velocity>(oss, entityData.name, entityData.component);
    if (entityData.name == ecs::ComponentName::SPRITEINFOS)
        network::HandleData::serializeEntityCompotent<ecs::component::SpriteInfos>(oss, entityData.name, entityData.component);
    if (entityData.name == ecs::ComponentName::SHOOTING)
        network::HandleData::serializeEntityCompotent<ecs::component::Shooting>(oss, entityData.name, entityData.component);
    if (entityData.name == ecs::ComponentName::LIFE)
        network::HandleData::serializeEntityCompotent<ecs::component::Life>(oss, entityData.name, entityData.component);
    if (entityData.name == ecs::ComponentName::TYPE)
        network::HandleData::serializeEntityCompotent<ecs::component::Type>(oss, entityData.name, entityData.component);
    if (entityData.name == ecs::ComponentName::SOUNDEFFECT)
        network::HandleData::serializeEntityCompotent<ecs::component::SoundEffectInfos>(oss, entityData.name, entityData.component);
    if (entityData.name == ecs::ComponentName::MUSIC)
        network::HandleData::serializeEntityCompotent<ecs::component::BackgroundMusicInfos>(oss, entityData.name, entityData.component);
    if (entityData.name == ecs::ComponentName::SCORE)
        network::HandleData::serializeEntityCompotent<ecs::component::Score>(oss, entityData.name, entityData.component);
    if (entityData.name == ecs::ComponentName::TEXTINFOS)
        network::HandleData::serializeEntityCompotent<ecs::component::TextInfos>(oss, entityData.name, entityData.component);
    if (entityData.name == ecs::ComponentName::ACTIVEPOWERUP)
        network::HandleData::serializeEntityCompotent<ecs::component::ActivePowerup>(oss, entityData.name, entityData.component);
    if (entityData.name == ecs::ComponentName::MOVEMENTPATTERN)
        network::HandleData::serializeEntityCompotent<ecs::component::MovementPattern>(oss, entityData.name, entityData.component);
    if (entityData.name == ecs::ComponentName::PLAYERWEAPON)
        network::HandleData::serializeEntityCompotent<ecs::component::Weapon>(oss, entityData.name, entityData.component);
    oss << '\n';
}

void operator>>(std::istringstream &iss, ecs::EntityType &type) {
    int tmpType = 0;

    iss >> tmpType;
    type = static_cast<ecs::EntityType>(tmpType);
}

void operator>>(std::istringstream &iss, ecs::MovementType &movementType) {
    int tmpType = 0;

    iss >> tmpType;
    movementType = static_cast<ecs::MovementType>(tmpType);
}

void operator>>(std::istringstream &iss, ecs::WeaponType &weaponType) {
    int tmpType = 0;

    iss >> tmpType;
    weaponType = static_cast<ecs::WeaponType>(tmpType);
}

void operator>>(std::istringstream &iss, ecs::ComponentName &name) {
    int tmpName = 0;

    iss >> tmpName;
    name = static_cast<ecs::ComponentName>(tmpName);
}

void operator>>(std::istringstream &iss, ecs::component::Position &position) {
    iss >> position.x >> position.y >> position.startX >> position.startY;
}

void operator>>(std::istringstream &iss, ecs::component::Velocity &velocity) {
    iss >> velocity.vx >> velocity.vy >> velocity.possibleVelocity;
}

void operator>>(std::istringstream &iss, ecs::component::SpriteInfos &spriteInfos) {
    iss >> spriteInfos.patternNb >> spriteInfos.firstPattern >> spriteInfos.scaleFactor >> spriteInfos.spritePath >> spriteInfos.color.a >>
        spriteInfos.color.r >> spriteInfos.color.g >> spriteInfos.color.b >> spriteInfos.spriteWidth >> spriteInfos.spriteHeight >>
        spriteInfos.rotationAngle;
}

void operator>>(std::istringstream &iss, ecs::component::Shooting &shooting) {
    iss >> shooting.isShooting >> shooting.shootPower >> shooting.shootSize;
}

void operator>>(std::istringstream &iss, ecs::component::Type &type) {
    iss >> type.entityType;
}

void operator>>(std::istringstream &iss, ecs::component::Life &life) {
    iss >> life.nbLives >> life.displayLife >> life.maxLives >> life.isInvincible;
}

void operator>>(std::istringstream &iss, ecs::component::SoundEffectInfos &soundEffectInfos) {
    iss >> soundEffectInfos.isLoaded >> soundEffectInfos.hasToBePlayed >> soundEffectInfos.soundPath;
}

void operator>>(std::istringstream &iss, ecs::component::BackgroundMusicInfos &music) {
    iss >> music.musicPath >> music.volume >> music.hasToBePlayed >> music.isLoaded >> music.isCurrentlyPlayed;
}

void operator>>(std::istringstream &iss, ecs::component::Score &score) {
    iss >> score.score >> score.scoreIncrementation;
}

void operator>>(std::istringstream &iss, ecs::component::TextInfos &textInfo) {
    iss >> textInfo.textToDisplay >> textInfo.x >> textInfo.y >> textInfo.size >> textInfo.fontPath >> textInfo.color.a >>
        textInfo.color.r >> textInfo.color.g >> textInfo.color.b;
}

void operator>>(std::istringstream &iss, ecs::component::ActivePowerup &activePowerup) {
    iss >> activePowerup.isPowershootActive >> activePowerup.isInvincibleActive >> activePowerup.isMegaShootActive;
}

void operator>>(std::istringstream &iss, ecs::component::MovementPattern &movementPattern) {
    iss >> movementPattern.patternAmplitude;
    iss >> movementPattern.isReversible;
    iss >> movementPattern.movementPatternType;
}

void operator>>(std::istringstream &iss, ecs::component::Weapon &weapons) {
    iss >> weapons.weaponType;
    iss >> weapons.playerIndex;
}

void operator>>(std::istringstream &iss, EntityData &entityData) {

    iss >> entityData.entityID;
    iss >> entityData.name;
    if (entityData.name == ecs::ComponentName::POSITION)
        network::HandleData::deserializeEntityCompotent<ecs::component::Position>(iss, entityData.name, entityData.component);
    if (entityData.name == ecs::ComponentName::VELOCITY)
        network::HandleData::deserializeEntityCompotent<ecs::component::Velocity>(iss, entityData.name, entityData.component);
    if (entityData.name == ecs::ComponentName::SPRITEINFOS)
        network::HandleData::deserializeEntityCompotent<ecs::component::SpriteInfos>(iss, entityData.name, entityData.component);
    if (entityData.name == ecs::ComponentName::SHOOTING)
        network::HandleData::deserializeEntityCompotent<ecs::component::Shooting>(iss, entityData.name, entityData.component);
    if (entityData.name == ecs::ComponentName::LIFE)
        network::HandleData::deserializeEntityCompotent<ecs::component::Life>(iss, entityData.name, entityData.component);
    if (entityData.name == ecs::ComponentName::TYPE)
        network::HandleData::deserializeEntityCompotent<ecs::component::Type>(iss, entityData.name, entityData.component);
    if (entityData.name == ecs::ComponentName::SOUNDEFFECT)
        network::HandleData::deserializeEntityCompotent<ecs::component::SoundEffectInfos>(iss, entityData.name, entityData.component);
    if (entityData.name == ecs::ComponentName::MUSIC)
        network::HandleData::deserializeEntityCompotent<ecs::component::BackgroundMusicInfos>(iss, entityData.name, entityData.component);
    if (entityData.name == ecs::ComponentName::SCORE)
        network::HandleData::deserializeEntityCompotent<ecs::component::Score>(iss, entityData.name, entityData.component);
    if (entityData.name == ecs::ComponentName::TEXTINFOS)
        network::HandleData::deserializeEntityCompotent<ecs::component::TextInfos>(iss, entityData.name, entityData.component);
    if (entityData.name == ecs::ComponentName::ACTIVEPOWERUP)
        network::HandleData::deserializeEntityCompotent<ecs::component::ActivePowerup>(iss, entityData.name, entityData.component);
    if (entityData.name == ecs::ComponentName::MOVEMENTPATTERN)
        network::HandleData::deserializeEntityCompotent<ecs::component::MovementPattern>(iss, entityData.name, entityData.component);
    if (entityData.name == ecs::ComponentName::PLAYERWEAPON)
        network::HandleData::deserializeEntityCompotent<ecs::component::Weapon>(iss, entityData.name, entityData.component);
}
