//
// Created by igor on 2024.07.06..
//

#include "player.h"

#ifndef RAYLIB_GAME_TEMPLATE_PLAYEREFFECT_H
#define RAYLIB_GAME_TEMPLATE_PLAYEREFFECT_H

/*
 * IMPORTANT!!!!!!!!!!!!!!!!!!!!!!!!!!
 * Buffs are even, Debuffs are odd. This means that the list is sturctured as follows:
 * Buff
 * Debuff
 * Buff
 * Debuff
 * etc.
 */

static int playerEffectCount = 14;

typedef enum {
    MORE_PLAYER_SPEED = 0,
    LESS_PLAYER_SPEED = 1,
    MORE_BULLET_SPEED,
    LESS_BULLET_SPEED,
    SMALLER_BODY,
    LARGER_BODY,
    MORE_BOOLET_AMPLITUDE,
    LESS_BOOLET_AMPLITUDE,
    MORE_MAX_HEALTH,
    LESS_MAX_HEALTH,
    MORE_FRICTION,
    LESS_FRICTION,
    SHORTER_SHOOT_COOLDOWN_TIME,
    LONGER_SHOOT_COOLDOWN_TIME,
    CLEAR_ALL_EFFECTS
} PlayerEffect;
static const char playerEffectNames[][24] = {
        [MORE_PLAYER_SPEED] = "SPEED DEMON",
        [LESS_PLAYER_SPEED] = "SUNDAY DRIVER",
        [MORE_BULLET_SPEED] = "FAST PIECE",
        [LESS_BULLET_SPEED] = "SLOW PIECE",
        [SMALLER_BODY] = "ANT",
        [LARGER_BODY] = "ANT EATER",
        [MORE_BOOLET_AMPLITUDE] = "MORE SWAY",
        [LESS_BOOLET_AMPLITUDE] = "LESS SWAY",
        [MORE_MAX_HEALTH] = "HEALTH UP",
        [LESS_MAX_HEALTH] = "ATTRITION",
        [MORE_FRICTION] = "CRAMPONS",
        [LESS_FRICTION] = "ICE SKATES",
        [SHORTER_SHOOT_COOLDOWN_TIME] = "QUICK HANDS",
        [LONGER_SHOOT_COOLDOWN_TIME] = "SLOW HANDS",
        [CLEAR_ALL_EFFECTS] = "RM -RF EFFECTS"
};

static const int player_speed_modifier = 150;
static const int boolet_speed_modifier = 150;
static const int body_size_modifier = 10;
static const int boolet_amplitude_modifier = 2;
static const int max_health_modifier = 1;
static const float friction_modifier = 0.075;
static const float shoot_cooldown_time_modifier = 0.2;

// -------------------------------------------------------------------

void AssignEffectToPlayer(PlayerEffect playerEffect, struct Player *p);

void applyEffectToPlayer(PlayerEffect effectToApply, struct Player *p);

void undoEffectFromPlayer(PlayerEffect effectToUndo, struct Player *p);

void IncreaseEffectToSwapIndex(struct Player *p);

int getRandomBuff();

int getRandomDebuff();

void ClearPlayerOfEffects(struct Player *p);

#endif //RAYLIB_GAME_TEMPLATE_PLAYEREFFECT_H
