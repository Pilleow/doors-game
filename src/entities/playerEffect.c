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
            p->defaultSpeed += player_speed_modifier * playerEffectMultiplier;
            p->speed += player_speed_modifier * playerEffectMultiplier;
            break;
        case LESS_PLAYER_SPEED:
            p->defaultSpeed -= player_speed_modifier;
            p->speed -= player_speed_modifier;
            break;
        case MORE_BULLET_SPEED:
            p->bulletSpeed += boolet_speed_modifier * playerEffectMultiplier / 2;
            break;
        case LESS_BULLET_SPEED:
            p->bulletSpeed -= boolet_speed_modifier;
            break;
        case SMALLER_BODY:
            float pef = body_size_modifier * playerEffectMultiplier / 2;
            if (p->rect.width > pef && p->rect.height > pef) {
                p->rect.width -= pef;
                p->rect.height -= pef;
            } else p->activeEffects[p->nextEffectToSwapIndex] = -1;
            break;
        case LARGER_BODY:
            p->rect.width += body_size_modifier * playerEffectMultiplier;
            p->rect.height += body_size_modifier * playerEffectMultiplier;
            break;
        case MORE_BOOLET_AMPLITUDE:
            p->booletAmplitude += boolet_amplitude_modifier * playerEffectMultiplier;
            break;
        case LESS_BOOLET_AMPLITUDE:
            p->booletAmplitude -= boolet_amplitude_modifier * playerEffectMultiplier;
            break;
        case MORE_MAX_HEALTH:
            p->maxHealth += max_health_modifier * playerEffectMultiplier;
            break;
        case LESS_MAX_HEALTH:
            if (p->maxHealth > 1) p->maxHealth -= max_health_modifier * playerEffectMultiplier;
            else p->activeEffects[p->nextEffectToSwapIndex] = -1;
            break;
        case LESS_FRICTION:
            p->friction += friction_modifier * playerEffectMultiplier;
            break;
        case MORE_FRICTION:
            p->friction -= friction_modifier * playerEffectMultiplier;
            break;
        case SHORTER_SHOOT_COOLDOWN_TIME:
            p->shotCooldownTime -= shoot_cooldown_time_modifier * playerEffectMultiplier;
            break;
        case LONGER_SHOOT_COOLDOWN_TIME:
            p->shotCooldownTime += shoot_cooldown_time_modifier * playerEffectMultiplier;
            break;
        case SHORTER_DASH_COOLDOWN:
            p->dodgeCooldownTime -= dodge_cooldown_time_modifier * playerEffectMultiplier;
            break;
        case LONGER_DASH_COOLDOWN:
            p->dodgeCooldownTime += dodge_cooldown_time_modifier * playerEffectMultiplier;
            break;
        case LARGER_BULLET_SIZE:
            p->booletSize += boolet_size_modifier * playerEffectMultiplier;
            break;
        case SMALLER_BULLET_SIZE:
            p->booletSize -= boolet_size_modifier * playerEffectMultiplier;
            break;
        default:
            break;
    }
}

void undoEffectFromPlayer(PlayerEffect effectToUndo, struct Player *p) {
    switch (effectToUndo) {
        case MORE_PLAYER_SPEED:
            p->defaultSpeed -= player_speed_modifier * playerEffectMultiplier;
            p->speed -= player_speed_modifier * playerEffectMultiplier;
            break;
        case LESS_PLAYER_SPEED:
            p->defaultSpeed += player_speed_modifier;
            p->speed += player_speed_modifier;
            break;
        case MORE_BULLET_SPEED:
            p->bulletSpeed -= boolet_speed_modifier * playerEffectMultiplier / 2;
            break;
        case LESS_BULLET_SPEED:
            p->bulletSpeed += boolet_speed_modifier;
            break;
        case SMALLER_BODY:
            p->rect.width += body_size_modifier * playerEffectMultiplier / 2;
            p->rect.height += body_size_modifier * playerEffectMultiplier / 2;
            break;
        case LARGER_BODY:
            p->rect.width -= body_size_modifier * playerEffectMultiplier;
            p->rect.height -= body_size_modifier * playerEffectMultiplier;
            break;
        case MORE_BOOLET_AMPLITUDE:
            p->booletAmplitude -= boolet_amplitude_modifier * playerEffectMultiplier;
            break;
        case LESS_BOOLET_AMPLITUDE:
            p->booletAmplitude += boolet_amplitude_modifier * playerEffectMultiplier;
            break;
        case MORE_MAX_HEALTH:
            if (p->maxHealth > 1) p->maxHealth -= max_health_modifier * playerEffectMultiplier;
            break;
        case LESS_MAX_HEALTH:
            p->maxHealth += max_health_modifier * playerEffectMultiplier;
            break;
        case LESS_FRICTION:
            p->friction -= friction_modifier * playerEffectMultiplier;
            break;
        case MORE_FRICTION:
            p->friction += friction_modifier * playerEffectMultiplier;
            break;
        case SHORTER_SHOOT_COOLDOWN_TIME:
            p->shotCooldownTime += shoot_cooldown_time_modifier * playerEffectMultiplier;
            break;
        case LONGER_SHOOT_COOLDOWN_TIME:
            p->shotCooldownTime -= shoot_cooldown_time_modifier * playerEffectMultiplier;
            break;
        case SHORTER_DASH_COOLDOWN:
            p->dodgeCooldownTime += dodge_cooldown_time_modifier * playerEffectMultiplier;
            break;
        case LONGER_DASH_COOLDOWN:
            p->dodgeCooldownTime -= dodge_cooldown_time_modifier * playerEffectMultiplier;
            break;
        case LARGER_BULLET_SIZE:
            p->booletSize -= boolet_size_modifier * playerEffectMultiplier;
            break;
        case SMALLER_BULLET_SIZE:
            p->booletSize += boolet_size_modifier * playerEffectMultiplier;
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
