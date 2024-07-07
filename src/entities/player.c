#include "raylib.h"
#include "raymath.h"

#include "player.h"
#include "../constants.h"

#include <stdio.h>

// this should only be used once on first initialization
void InitPlayerDefaults(
        struct Player *p, Location atCorner,
        KeyboardKey up, KeyboardKey down,
        KeyboardKey left, KeyboardKey right,
        KeyboardKey upShoot, KeyboardKey downShoot,
        KeyboardKey leftShoot, KeyboardKey rightShoot,
        KeyboardKey dodge
) {
    SetPlayerLocation(p, atCorner);
    p->rect.height = 40;
    p->rect.width = 40;
    p->velocity.x = 0;
    p->velocity.y = 0;
    p->shootingDirection.x = 0;
    p->shootingDirection.y = 0;
    p->maxHealth = 2;
    p->health = p->maxHealth;
    p->defaultSpeed = 350;
    p->speed = p->defaultSpeed;
    p->keyMoveUp = up;
    p->keyMoveDown = down;
    p->keyMoveLeft = left;
    p->keyMoveRight = right;
    p->keyShootUp = upShoot;
    p->keyShootDown = downShoot;
    p->keyShootLeft = leftShoot;
    p->keyShootRight = rightShoot;
    p->keyDodge = dodge;
    p->isDead = false;
    p->isPlaying = true;
    p->startLocation = atCorner;
    p->wins = -1;
    AddWinToPlayer(p);
    p->huePhase = (atCorner * 60) % 360;
    p->lastShotTime = GetTime();
    p->shotCooldownTime = 0.5F;
    p->friction = 0.91F;
    p->bulletSpeed = 600;
    p->lastDodgeTime = GetTime();
    p->dodgeCooldownTime = 1.5F;
    p->nextEffectToSwapIndex = 0;
    for (int i = 0; i < playerEffectCapacityAndLifespan; ++i) p->activeEffects[i] = -1;
}

void printDebugMessage(struct Player *p) {
    printf("Player Info: hue:%d, rect[%.2f, %.2f, %.2f, %.2f], vel[%.2f, %.2f], HP:%d/%d, spd:%d/%d, bulSpd:%d, dead:%d, shot:%.2f/%.2f, dodge:%.2f/%.2f, fric:%.2f, sLoc:%d, eff[%d, %d, %d]\n",
           p->huePhase,
           p->rect.x, p->rect.y, p->rect.width, p->rect.height,
           p->velocity.x, p->velocity.y,
           p->health, p->maxHealth,
           p->speed, p->defaultSpeed,
           p->bulletSpeed, p->isDead,
           p->lastShotTime, p->shotCooldownTime,
           p->lastDodgeTime, p->dodgeCooldownTime,
           p->friction, p->startLocation,
           p->activeEffects[0], p->activeEffects[1], p->activeEffects[2]);
}


void SetPlayerLocation(struct Player *p, Location atCorner) {
    int textWidth = MeasureText(p->winsString, winsFontSize);
    int padding = 15;
    switch (atCorner) {
        case TOPLEFT:
            p->rect.x = 0.1F * screenWidth - p->rect.width / 2;
            p->rect.y = 0.1F * screenHeight - p->rect.height / 2;
            p->winsStringX = padding;
            p->winsStringY = padding;
            break;
        case TOPRIGHT:
            p->rect.x = 0.9F * screenWidth - p->rect.width / 2;
            p->rect.y = 0.1F * screenHeight - p->rect.height / 2;
            p->winsStringX = screenWidth - padding - textWidth;
            p->winsStringY = padding;
            break;
        case BOTTOMLEFT:
            p->rect.x = 0.1F * screenWidth - p->rect.width / 2;
            p->rect.y = 0.9F * screenHeight - p->rect.height / 2;
            p->winsStringX = padding;
            p->winsStringY = screenHeight - padding - winsFontSize;
            break;
        case BOTTOMRIGHT:
            p->rect.x = 0.9F * screenWidth - p->rect.width / 2;
            p->rect.y = 0.9F * screenHeight - p->rect.height / 2;
            p->winsStringX = screenWidth - padding - textWidth;
            p->winsStringY = screenHeight - padding - winsFontSize;
            break;
        default:
            p->rect.x = screenWidth / 2 - p->rect.width / 2;
            p->rect.y = screenHeight / 2 - p->rect.height / 2;
            p->winsStringX = screenWidth / 2;
            p->winsStringY = screenHeight / 2;
    }
}

void ProcessPlayerInput(struct Player *p) {
    p->velocity.x *= p->friction;
    p->velocity.y *= p->friction;
    if (IsKeyDown(p->keyMoveUp)) p->velocity.y -= 1;
    if (IsKeyDown(p->keyMoveDown)) p->velocity.y += 1;
    if (IsKeyDown(p->keyMoveLeft)) p->velocity.x -= 1;
    if (IsKeyDown(p->keyMoveRight)) p->velocity.x += 1;
    if (Vector2LengthSqr(p->velocity) > 1)
        p->velocity = Vector2Normalize(p->velocity);

    if (p->speed > p->defaultSpeed) p->speed =  p->defaultSpeed + 0.925 * (p->speed - p->defaultSpeed);
    if (IsKeyDown(p->keyDodge) && GetTime() - p->lastDodgeTime > p->dodgeCooldownTime) {
        p->lastDodgeTime = GetTime();
        p->speed *= 4;
    }

    p->shootingDirection.x = 0;
    p->shootingDirection.y = 0;
    if (IsKeyDown(p->keyShootUp)) p->shootingDirection.y -= 1;
    if (IsKeyDown(p->keyShootDown)) p->shootingDirection.y += 1;
    if (IsKeyDown(p->keyShootLeft)) p->shootingDirection.x -= 1;
    if (IsKeyDown(p->keyShootRight)) p->shootingDirection.x += 1;
    p->shootingDirection = Vector2Normalize(p->shootingDirection);
}

void ApplyPlayerVelocity(struct Player *p) {
    float frameTime = GetFrameTime();
    if (p->velocity.x != 0) p->rect.x += p->velocity.x * p->speed * frameTime;
    if (p->velocity.y != 0) p->rect.y += p->velocity.y * p->speed * frameTime;

    if (p->rect.x < 0) p->rect.x = 0;
    else if (p->rect.x + p->rect.width > screenWidth) p->rect.x = screenWidth - p->rect.width;
    if (p->rect.y < 0) p->rect.y = 0;
    else if (p->rect.y + p->rect.height > screenHeight) p->rect.y = screenHeight - p->rect.height;
}

void ApplyDamageToPlayer(struct Player *p, unsigned char damage) {
    if (damage >= p->health) {
        p->health = 0;
        p->isDead = true;
    } else p->health -= damage;
}

void DrawPlayer(struct Player *p) {
    DrawRectangle(p->rect.x, p->rect.y, p->rect.width, p->rect.height, ColorFromHSV(p->huePhase, 1, 1));
    float s = 1 - (float) p->health / (float) p->maxHealth;
    DrawRectangle(p->rect.x + p->rect.width / 2 * (1 - s), p->rect.y + p->rect.height / 2 * (1 - s), p->rect.width * s,
                  p->rect.height * s,ColorFromHSV(180, 0.1, 0.1));
}

void AddWinToPlayer(struct Player *p) {
    p->wins += 1;
    sprintf(p->winsString, "%d", p->wins);

    int textWidth = MeasureText(p->winsString, winsFontSize);
    int padding = 15;
    switch (p->startLocation) {
        case TOPLEFT:
            p->winsStringX = padding;
            p->winsStringY = padding;
            break;
        case BOTTOMLEFT:
            p->winsStringX = padding;
            p->winsStringY = screenHeight - padding - winsFontSize;
            break;
        case TOPRIGHT:
            p->winsStringX = screenWidth - padding - textWidth;
            p->winsStringY = padding;
            break;
        case BOTTOMRIGHT:
            p->winsStringX = screenWidth - padding - textWidth;
            p->winsStringY = screenHeight - padding - winsFontSize;
            break;
        default:
            p->winsStringX = (screenWidth - textWidth) / 2;
            p->winsStringY = (screenHeight - winsFontSize) / 2;
    }
}

void DrawPlayerScore(struct Player *p) {
    DrawText(p->winsString, p->winsStringX, p->winsStringY, winsFontSize, ColorFromHSV(p->huePhase, 0.7, 0.7));
}

bool IsPlayerShooting(struct Player *p) {
    if (GetTime() - p->lastShotTime > p->shotCooldownTime &&
        (p->shootingDirection.x != 0 || p->shootingDirection.y != 0)) {
        p->lastShotTime = GetTime();
        return true;
    }
    return false;
}
