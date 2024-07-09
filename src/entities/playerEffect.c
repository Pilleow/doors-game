//
// Created by igor on 2024.07.06..
//

#include "playerEffect.h"
#include "../constants.h"

#include <stdlib.h>

void AssignEffectToPlayer(PlayerEffect playerEffect, struct Player *p) {

    undoEffectFromPlayer(p->activeEffects[p->nextEffectToSwapIndex], p);
    p->activeEffects[p->nextEffectToSwapIndex] = playerEffect;
    applyEffectToPlayer(p->activeEffects[p->nextEffectToSwapIndex], p);
    IncreaseEffectToSwapIndex(p);
}

void applyEffectToPlayer(PlayerEffect effectToApply, struct Player *p) {
    switch (effectToApply) {
        case MORE_PLAYER_SPEED:
            p->defaultSpeed += player_speed_modifier;
            p->speed += player_speed_modifier;
            break;
        case LESS_PLAYER_SPEED:
            p->defaultSpeed -= player_speed_modifier;
            p->speed -= player_speed_modifier;
            break;
        case MORE_BULLET_SPEED:
            p->bulletSpeed += boolet_speed_modifier;
            break;
        case LESS_BULLET_SPEED:
            p->bulletSpeed -= boolet_speed_modifier;
            break;
        case SMALLER_BODY:
            p->rect.width -= body_size_modifier;
            p->rect.height -= body_size_modifier;
            break;
        case LARGER_BODY:
            p->rect.width += body_size_modifier;
            p->rect.height += body_size_modifier;
            break;
        case MORE_BOOLET_AMPLITUDE:
            p->booletAmplitude += boolet_amplitude_modifier;
            break;
        case LESS_BOOLET_AMPLITUDE:
            p->booletAmplitude -= boolet_amplitude_modifier;
            break;
        case MORE_MAX_HEALTH:
            p->maxHealth += max_health_modifier;
            break;
        case LESS_MAX_HEALTH:
            if (p->maxHealth > 1) p->maxHealth -= max_health_modifier;
            break;
        case LESS_FRICTION:
            p->friction += friction_modifier;
            break;
        case MORE_FRICTION:
            p->friction -= friction_modifier;
            break;
        case SHORTER_SHOOT_COOLDOWN_TIME:
            p->shotCooldownTime -= shoot_cooldown_time_modifier;
            break;
        case LONGER_SHOOT_COOLDOWN_TIME:
            p->shotCooldownTime += shoot_cooldown_time_modifier;
            break;
        case SHORTER_DASH_COOLDOWN:
            p->dodgeCooldownTime -= dodge_cooldown_time_modifier;
            break;
        case LONGER_DASH_COOLDOWN:
            p->dodgeCooldownTime += dodge_cooldown_time_modifier;
            break;
        default:
            break;
    }
}

void undoEffectFromPlayer(PlayerEffect effectToUndo, struct Player *p) {
    switch (effectToUndo) {
        case MORE_PLAYER_SPEED:
            p->defaultSpeed -= player_speed_modifier;
            p->speed -= player_speed_modifier;
            break;
        case LESS_PLAYER_SPEED:
            p->defaultSpeed += player_speed_modifier;
            p->speed += player_speed_modifier;
            break;
        case MORE_BULLET_SPEED:
            p->bulletSpeed -= boolet_speed_modifier;
            break;
        case LESS_BULLET_SPEED:
            p->bulletSpeed += boolet_speed_modifier;
            break;
        case SMALLER_BODY:
            p->rect.width += body_size_modifier;
            p->rect.height += body_size_modifier;
            break;
        case LARGER_BODY:
            p->rect.width -= body_size_modifier;
            p->rect.height -= body_size_modifier;
            break;
        case MORE_BOOLET_AMPLITUDE:
            p->booletAmplitude -= boolet_amplitude_modifier;
            break;
        case LESS_BOOLET_AMPLITUDE:
            p->booletAmplitude += boolet_amplitude_modifier;
            break;
        case MORE_MAX_HEALTH:
            if (p->maxHealth > 1) p->maxHealth -= max_health_modifier;
            break;
        case LESS_MAX_HEALTH:
            p->maxHealth += max_health_modifier;
            break;
        case LESS_FRICTION:
            p->friction -= friction_modifier;
            break;
        case MORE_FRICTION:
            p->friction += friction_modifier;
            break;
        case SHORTER_SHOOT_COOLDOWN_TIME:
            p->shotCooldownTime += shoot_cooldown_time_modifier;
            break;
        case LONGER_SHOOT_COOLDOWN_TIME:
            p->shotCooldownTime -= shoot_cooldown_time_modifier;
            break;
        case SHORTER_DASH_COOLDOWN:
            p->dodgeCooldownTime += dodge_cooldown_time_modifier;
            break;
        case LONGER_DASH_COOLDOWN:
            p->dodgeCooldownTime -= dodge_cooldown_time_modifier;
            break;
        default:
            break;
    }
}

int getRandomBuff() {
    return 2 * (rand() % (playerEffectCount / 2));
}

int getRandomDebuff() {
    return 2 * (rand() % (playerEffectCount / 2)) + 1;
}

int getRandomSpecialEffect() {
    return playerEffectCount + (rand() % specialPlayerEffectCount) - 1;
}

void IncreaseEffectToSwapIndex(struct Player *p) {
    p->nextEffectToSwapIndex = (p->nextEffectToSwapIndex + 1) % playerEffectCapacityAndLifespan;
}

void ClearPlayerOfEffects(struct Player *p) {
    for (int i = 0; i < playerEffectCapacityAndLifespan; ++i) {
        undoEffectFromPlayer(p->activeEffects[i], p);
        p->activeEffects[i] = -1;
    }
    p->nextEffectToSwapIndex = 0;
}
