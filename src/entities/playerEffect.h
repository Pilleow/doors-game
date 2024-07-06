//
// Created by igor on 2024.07.06..
//

#include "player.h"

#ifndef RAYLIB_GAME_TEMPLATE_PLAYEREFFECT_H
#define RAYLIB_GAME_TEMPLATE_PLAYEREFFECT_H

/*
 * IMPORTANT!!!!!!!!!!!!!!!!!!!!!!!!!!
 * Buffs are odd, Debuffs are even. This means that the list is sturctured as follows:
 * Buff
 * Debuff
 * Buff
 * Debuff
 * etc.
 */

static int playerEffectCount = 17;

typedef enum {
    MORE_PLAYER_SPEED = 0,
    LESS_PLAYER_SPEED = 1,
    MORE_BULLET_SPEED,
    LESS_BULLET_SPEED,
    SMALLER_BODY,
    LARGER_BODY,
    LESS_RANDOM_BULLET_SPREAD,
    MORE_RANDOM_BULLET_SPREAD,
    MORE_MAX_HEALTH,
    LESS_MAX_HEALTH,
    LESS_FRICTION,
    MORE_FRICTION,
    SHORTER_SHOOT_COOLDOWN_TIME,
    LONGER_SHOOT_COOLDOWN_TIME,
    EVERYONE_GETS_RANDOM_BUFFS,
    EVERYONE_GETS_RANDOM_DEBUFFS,
    EVERYONE_GETS_RANDOM_EFFECTS,
    CLEAR_ALL_EFFECTS
} PlayerEffect;

static const int player_speed_modifier = 100;
static const int bullet_speed_modifier = 100;
static const int body_size_modifier = 10;
static const int bullet_spread_modifier = 0; // this is not implemented yet! todo implement this
static const int max_health_modifier = 1;
static const float friction_modifier = 1/16 + 1/32;
static const float shoot_cooldown_time_modifier = 1/16;

// -------------------------------------------------------------------

void AssignEffectToPlayer(PlayerEffect playerEffect, struct Player *p);

void applyEffectToPlayer(PlayerEffect effectToApply, struct Player *p);

void undoEffectFromPlayer(PlayerEffect effectToUndo, struct Player *p);

void IncreaseEffectToSwapIndex(struct Player *p);

int getRandomBuff();

int getRandomDebuff();

void ClearPlayerOfEffects(struct Player *p);

#endif //RAYLIB_GAME_TEMPLATE_PLAYEREFFECT_H
