//
// Created by igor on 2024.07.06..
//

#include "playerEffect.h"
#include "../constants.h"

#include <stdlib.h>
#include <stdio.h>

void AssignEffectToPlayer(PlayerEffect playerEffect, struct Player *p) {

    undoEffectFromPlayer(p->activeEffects[p->nextEffectToSwapIndex], p);

    p->activeEffects[p->nextEffectToSwapIndex] = playerEffect;
    IncreaseEffectToSwapIndex(p);

    applyEffectToPlayer(p->activeEffects[p->nextEffectToSwapIndex], p);
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
            p->bulletSpeed += bullet_speed_modifier;
            break;
        case LESS_BULLET_SPEED:
            p->bulletSpeed -= bullet_speed_modifier;
            break;
        case SMALLER_BODY:
            p->rect.width -= body_size_modifier;
            p->rect.height -= body_size_modifier;
            break;
        case LARGER_BODY:
            p->rect.width += body_size_modifier;
            p->rect.height += body_size_modifier;
            break;
        case LESS_RANDOM_BULLET_SPREAD:
            break;
        case MORE_RANDOM_BULLET_SPREAD:
            break;
        case MORE_MAX_HEALTH:
            p->maxHealth += max_health_modifier;
            break;
        case LESS_MAX_HEALTH:
            p->maxHealth -= max_health_modifier;
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
        case EVERYONE_GETS_RANDOM_BUFFS:
            applyEffectToPlayer(rand() % playerEffectCapacityAndLifespan, p);
            break;
        case EVERYONE_GETS_RANDOM_DEBUFFS:
            applyEffectToPlayer(rand() % playerEffectCapacityAndLifespan, p);
            break;
        case EVERYONE_GETS_RANDOM_EFFECTS:
            applyEffectToPlayer(rand() % playerEffectCapacityAndLifespan, p);
            break;
        default:
            printf("[ERROR] Invalid effect provided to applyEffectToPlayer method: %d .", effectToApply);
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
            p->bulletSpeed -= bullet_speed_modifier;
            break;
        case LESS_BULLET_SPEED:
            p->bulletSpeed += bullet_speed_modifier;
            break;
        case SMALLER_BODY:
            p->rect.width += body_size_modifier;
            p->rect.height += body_size_modifier;
            break;
        case LARGER_BODY:
            p->rect.width -= body_size_modifier;
            p->rect.height -= body_size_modifier;
            break;
        case LESS_RANDOM_BULLET_SPREAD:
            break;
        case MORE_RANDOM_BULLET_SPREAD:
            break;
        case MORE_MAX_HEALTH:
            p->maxHealth -= max_health_modifier;
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
        default:
            printf("[ERROR] Invalid effect provided to undoEffectFromPlayer method: %d .", effectToUndo);
    }
}

int getRandomBuff() {
    return 2 * (rand() % playerEffectCount / 2) + 1;
}

int getRandomDebuff() {
    return 2 * (rand() % playerEffectCount / 2);
}

void IncreaseEffectToSwapIndex(struct Player *p) {
    p->nextEffectToSwapIndex = (p->nextEffectToSwapIndex + 1) % playerEffectCapacityAndLifespan;
}

void ClearPlayerOfEffects(struct Player *p) {
    for (int i = 0; i < playerEffectCapacityAndLifespan; ++i) {
        undoEffectFromPlayer(p->activeEffects[i], p);
    }
    p->nextEffectToSwapIndex = 0;
}
