//
// Created by igor on 2024.07.06..
//

#include "../constants.h"
#include "raylib.h"

#ifndef RAYLIB_GAME_TEMPLATE_PLAYER_H
#define RAYLIB_GAME_TEMPLATE_PLAYER_H

struct Player {
    bool isDead;
    bool isPlaying;
    char winsString[16];
    unsigned char health;
    unsigned char maxHealth;
    unsigned short speed;
    unsigned short defaultSpeed;
    unsigned short bulletSpeed;
    unsigned short huePhase;
    int activeEffects[playerEffectCapacityAndLifespan];
    int nextEffectToSwapIndex;
    int wins;
    int winsStringX;
    int winsStringY;
    int booletAmplitude;
    int sfxShootSoundIndex;
    float lastPastPositionUpdateTime;
    float lastDodgeTime;
    float lastShotTime;
    float shotCooldownTime;
    float dodgeCooldownTime;
    float friction;
    float booletDecayTimeLeft;
    Location startLocation;
    BooletType booletType;
    KeyboardKey keyMoveUp;
    KeyboardKey keyMoveDown;
    KeyboardKey keyMoveLeft;
    KeyboardKey keyMoveRight;
    KeyboardKey keyShootUp;
    KeyboardKey keyShootDown;
    KeyboardKey keyShootLeft;
    KeyboardKey keyShootRight;
    KeyboardKey keyDodge;
    Vector2 velocity;
    Vector2 shootingDirection;
    Vector2 pastPos[pastPlayerPositionsCount];
    Rectangle rect;
};

void InitPlayerDefaults(
        struct Player *p, Location atCorner,
        KeyboardKey up, KeyboardKey down,
        KeyboardKey left, KeyboardKey right,
        KeyboardKey upShoot, KeyboardKey downShoot,
        KeyboardKey leftShoot, KeyboardKey rightShoot,
        KeyboardKey dodge
);

void MovePlayerBackOneStep(struct Player *b);

void AddWinToPlayer(struct Player *p);

void DrawPlayerScore(struct Player *p);

void printDebugMessage(struct Player *p);

void SetPlayerLocation(struct Player *p, Location atCorner);

void ProcessPlayerInput(struct Player *p, char gamepadId);

void ApplyPlayerVelocity(struct Player *p);

void ApplyDamageToPlayer(struct Player *p, unsigned char damage);

void DrawPlayerTail(struct Player *p);

void DrawPlayer(struct Player *p);

bool IsPlayerShooting(struct Player *p);

#endif //RAYLIB_GAME_TEMPLATE_PLAYER_H
