//
// Created by igor on 2024.07.06..
//

#include "player.h"
#include "raylib.h"

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

static int playerEffectCount = 17;
static int specialPlayerEffectCount = 1;

typedef enum {
    MORE_PLAYER_SPEED = 0,
    LESS_PLAYER_SPEED = 1,
    MORE_BOOLET_SPEED,
    LESS_BOOLET_SPEED,
    SMALLER_BODY,
    LARGER_BODY,
    MORE_BOOLET_AMPLITUDE,
    LESS_BOOLET_AMPLITUDE,
    MORE_MAX_HEALTH,
    LESS_MAX_HEALTH,
    MORE_FRICTION,
    LESS_FRICTION,
    SHORTER_SHOOT_COOLDOWN,
    LONGER_SHOOT_COOLDOWN,
    SHORTER_DASH_COOLDOWN,
    LONGER_DASH_COOLDOWN,
    RANDOM_EFFECT_TO_EVERYONE               // do not count this in playerEffectCount as this is a special case
} PlayerEffect;
static const char playerEffectNames[][17] = {
        [MORE_PLAYER_SPEED] = "SPEED DEMON",
        [LESS_PLAYER_SPEED] = "SUNDAY DRIVER",
        [MORE_BOOLET_SPEED] = "FAST PIECE",
        [LESS_BOOLET_SPEED] = "SLOW PIECE",
        [SMALLER_BODY] = "ANT",
        [LARGER_BODY] = "ANT EATER",
        [MORE_BOOLET_AMPLITUDE] = "MORE SWAY",
        [LESS_BOOLET_AMPLITUDE] = "LESS SWAY",
        [MORE_MAX_HEALTH] = "HEALTH UP",
        [LESS_MAX_HEALTH] = "ATTRITION",
        [MORE_FRICTION] = "CRAMPONS",
        [LESS_FRICTION] = "ICE SKATES",
        [SHORTER_SHOOT_COOLDOWN] = "QUICK HANDS",
        [LONGER_SHOOT_COOLDOWN] = "SLOW HANDS",
        [SHORTER_DASH_COOLDOWN] = "NEW BALANCE",
        [LONGER_DASH_COOLDOWN] = "OLD SNEAKERS",
        [RANDOM_EFFECT_TO_EVERYONE] = "???"
};
extern Texture2D playerEffectSprites[17];

static const int player_speed_modifier = 100;
static const int boolet_speed_modifier = 150;
static const int body_size_modifier = 15;
static const int boolet_amplitude_modifier = 2;
static const int max_health_modifier = 1;
static const float friction_modifier = 0.075;
static const float shoot_cooldown_time_modifier = 0.15;
static const float dodge_cooldown_time_modifier = 0.3;

// -------------------------------------------------------------------

void AssignEffectToPlayer(PlayerEffect playerEffect, struct Player *p);

void applyEffectToPlayer(PlayerEffect effectToApply, struct Player *p);

void undoEffectFromPlayer(PlayerEffect effectToUndo, struct Player *p);

void IncreaseEffectToSwapIndex(struct Player *p);

int getRandomBuff();

int getRandomDebuff();

int getRandomSpecialEffect();

void ClearPlayerOfEffects(struct Player *p);

#endif //RAYLIB_GAME_TEMPLATE_PLAYEREFFECT_H
