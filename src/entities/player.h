//
// Created by igor on 2024.07.06..
//

#include "../constants.h"
#include "raylib.h"

#ifndef RAYLIB_GAME_TEMPLATE_PLAYER_H
#define RAYLIB_GAME_TEMPLATE_PLAYER_H

struct Player {
    Rectangle rect;
    Vector2 velocity;
    Vector2 shootingDirection;
    unsigned char health;
    unsigned char maxHealth;
    unsigned short speed;
    unsigned short defaultSpeed;
    unsigned short bulletSpeed;
    bool isDead;
    unsigned short huePhase;
    float lastShotTime;
    float shotCooldownTime;
    float lastDodgeTime;
    float dodgeCooldownTime;
    float friction;
    int activeEffects[playerEffectCapacityAndLifespan];
    int nextEffectToSwapIndex;
    KeyboardKey keyMoveUp;
    KeyboardKey keyMoveDown;
    KeyboardKey keyMoveLeft;
    KeyboardKey keyMoveRight;
    KeyboardKey keyShootUp;
    KeyboardKey keyShootDown;
    KeyboardKey keyShootLeft;
    KeyboardKey keyShootRight;
    KeyboardKey keyDodge;
};

void InitPlayerDefaults(
        struct Player *p, Location atCorner,
        KeyboardKey up, KeyboardKey down,
        KeyboardKey left, KeyboardKey right,
        KeyboardKey upShoot, KeyboardKey downShoot,
        KeyboardKey leftShoot, KeyboardKey rightShoot,
        KeyboardKey dodge
);

void SetPlayerLocation(struct Player *p, Location atCorner);

void ProcessPlayerInput(struct Player *p);

void ApplyPlayerVelocity(struct Player *p);

void ApplyDamageToPlayer(struct Player *p, unsigned char damage);

void DrawPlayer(struct Player *p);

bool IsPlayerShooting(struct Player *p);

#endif //RAYLIB_GAME_TEMPLATE_PLAYER_H
