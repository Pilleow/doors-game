#include "raylib.h"
#include "raymath.h"
#include "../screens.h"

#include "player.h"
#include "../constants.h"

#include <stdio.h>
#include <stdlib.h>

// this should only be used once on first initialization
void InitPlayerDefaults(
        struct Player *p, Location atCorner,
        KeyboardKey up, KeyboardKey down,
        KeyboardKey left, KeyboardKey right,
        KeyboardKey upShoot, KeyboardKey downShoot,
        KeyboardKey leftShoot, KeyboardKey rightShoot,
        KeyboardKey dodge
) {
    p->startLocation = atCorner;
    SetPlayerLocation(p, p->startLocation);
    p->rect.height = 50;
    p->rect.width = 50;
    p->velocity.x = 0;
    p->velocity.y = 0;
    p->shootingDirection.x = 0;
    p->shootingDirection.y = 0;
    p->maxHealth = 2;
    p->health = p->maxHealth;
    p->defaultSpeed = 400;
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
    p->wins = -1;
    AddWinToPlayer(p);
    p->huePhase = (atCorner * 60) % 360;
    p->lastShotTime = GetTime();
    p->shotCooldownTime = 0.5F;
    p->booletType = HITSCAN;
    p->booletAmplitude = 3;
    p->friction = 0.91F;
    p->bulletSpeed = 600;
    p->lastDodgeTime = GetTime();
    p->dodgeCooldownTime = 1.5F;
    p->nextEffectToSwapIndex = 0;
    p->booletDecayTimeLeft = 1.3;
    for (int i = 0; i < playerEffectCapacityAndLifespan; ++i) p->activeEffects[i] = -1;
    p->lastPastPositionUpdateTime = GetTime();
    for (int i = 0; i < pastPlayerPositionsCount; ++i) {
        p->pastPos[i].x = p->rect.x;
        p->pastPos[i].y = p->rect.y;
    }
    p->sfxShootSoundIndex = rand() % sfxShootCount;
}

void printDebugMessage(struct Player *p) {
    printf("Player Info: hue:%d, rect[%.2f, %.2f, %.2f, %.2f], vel[%.2f, %.2f], HP:%d/%d, spd:%d/%d, bulSpd:%d, isDead:%d, shot:%.2f/%.2f, dodge:%.2f/%.2f, fric:%.2f, sLoc:%d, eff[%d, %d, %d]\n",
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

void MovePlayerBackOneStep(struct Player *b) {
    b->velocity.x *= -1;
    b->velocity.y *= -1;
    ApplyPlayerVelocity(b);
    b->velocity.x *= -1;
    b->velocity.y *= -1;
}

void ProcessPlayerInput(struct Player *p, char gamepadId) {
    if (p->speed < p->defaultSpeed * 1.1) {
        p->velocity.x *= p->friction;
        p->velocity.y *= p->friction;
    }
    if (!useGamepads) {
        if (IsKeyDown(p->keyMoveUp)) p->velocity.y -= 1;
        if (IsKeyDown(p->keyMoveDown)) p->velocity.y += 1;
        if (IsKeyDown(p->keyMoveLeft)) p->velocity.x -= 1;
        if (IsKeyDown(p->keyMoveRight)) p->velocity.x += 1;
    } else {
        float x = GetGamepadAxisMovement(gamepadId, GAMEPAD_AXIS_LEFT_X);
        float y = GetGamepadAxisMovement(gamepadId, GAMEPAD_AXIS_LEFT_Y);
        p->velocity.x = x == 0 ? p->velocity.x : x;
        p->velocity.y = y == 0 ? p->velocity.y : y;
    }

    if (Vector2LengthSqr(p->velocity) > 1)
        p->velocity = Vector2Normalize(p->velocity);

    if (p->speed > p->defaultSpeed) p->speed = p->defaultSpeed + (p->friction < 0.96 ? p->friction : 0.96) * (p->speed - p->defaultSpeed);
    if (
            ((!useGamepads && IsKeyDown(p->keyDodge)) ||
             (useGamepads && IsGamepadButtonDown(gamepadId, GAMEPAD_BUTTON_LEFT_TRIGGER_2)))
            && GetTime() - p->lastDodgeTime > p->dodgeCooldownTime
            ) {
        p->lastDodgeTime = GetTime();
        p->speed *= 4;
        PlaySound(sfxDash[rand() % sfxDashCount]);
    }

    p->shootingDirection.x = 0;
    p->shootingDirection.y = 0;
    if (!useGamepads) {
        if (IsKeyDown(p->keyShootUp)) p->shootingDirection.y -= 1;
        if (IsKeyDown(p->keyShootDown)) p->shootingDirection.y += 1;
        if (IsKeyDown(p->keyShootLeft)) p->shootingDirection.x -= 1;
        if (IsKeyDown(p->keyShootRight)) p->shootingDirection.x += 1;
    } else {
        p->shootingDirection.x = GetGamepadAxisMovement(gamepadId, GAMEPAD_AXIS_RIGHT_X);
        p->shootingDirection.y = GetGamepadAxisMovement(gamepadId, GAMEPAD_AXIS_RIGHT_Y);
    }
    p->shootingDirection = Vector2Normalize(p->shootingDirection);
}

Rectangle _getUpdatedRectByVelocity(Rectangle rect, Vector2 velocity, float speed) {
    float frameTime = GetFrameTime();
    if (velocity.x != 0) rect.x += velocity.x * speed * frameTime;
    if (velocity.y != 0) rect.y += velocity.y * speed * frameTime;

    if (rect.x < 0) rect.x = 0;
    else if (rect.x + rect.width > screenWidth) rect.x = screenWidth - rect.width;
    if (rect.y < 0) rect.y = 0;
    else if (rect.y + rect.height > screenHeight) rect.y = screenHeight - rect.height;
    return rect;
}

void ApplyPlayerVelocity(struct Player *p) {
    p->rect = _getUpdatedRectByVelocity(p->rect, p->velocity, p->speed);
}

void ApplyDamageToPlayer(struct Player *p, unsigned char damage) {
    if (damage >= p->health) {
        p->health = 0;
        p->isDead = true;
    } else p->health -= damage;
}

void DrawPlayerTail(struct Player *p) {
    if (GetTime() - p->lastPastPositionUpdateTime > 0.001) {
        p->lastPastPositionUpdateTime = GetTime();
        for (int i = pastPlayerPositionsCount - 1; i > 0; --i) {
            p->pastPos[i].x = p->pastPos[i - 1].x;
            p->pastPos[i].y = p->pastPos[i - 1].y;
        }
        p->pastPos[0].x = p->rect.x;
        p->pastPos[0].y = p->rect.y;
    }
    for (int i = pastPlayerPositionsCount - 1; i >= 0; --i) {
        if (i != 0 && p->pastPos[i - 1].x == p->pastPos[i].x && p->pastPos[i - 1].y == p->pastPos[i].y) continue;
        DrawRectangle(
                p->pastPos[i].x, p->pastPos[i].y,
                p->rect.width, p->rect.height,
                ColorAlpha(ColorFromHSV(p->huePhase, 1, 0.7), 1 - (float) i / (float) pastPlayerPositionsCount)
        );
    }
}

void DrawPlayer(struct Player *p) {
    if (p->shootingDirection.x != 0 || p->shootingDirection.y != 0) {
        int centerX = p->rect.x + p->rect.width / 2;
        int centerY = p->rect.y + p->rect.height / 2;
        int len = p->rect.width * 2;
        DrawRectangleV(
                (Vector2) {centerX + len * p->shootingDirection.x, centerY + len * p->shootingDirection.y},
                (Vector2) {10, 10},
                ColorFromHSV(p->huePhase, 1, 0.7)
        );
    }
    int pd = 3;
    DrawRectangle(p->rect.x, p->rect.y, p->rect.width, p->rect.height, ColorFromHSV(p->huePhase, 1, 0.7));
    DrawRectangle(p->rect.x + pd, p->rect.y + pd, p->rect.width - 2 * pd, p->rect.height - 2 * pd,
                  ColorFromHSV(p->huePhase, 1, 1));
    float s = 1 - (float) p->health / (float) p->maxHealth;
    DrawRectangle(p->rect.x + p->rect.width / 2 * (1 - s), p->rect.y + p->rect.height / 2 * (1 - s), p->rect.width * s,
                  p->rect.height * s, ColorFromHSV(180, 0.1, 0.1));
}

void AddWinToPlayer(struct Player *p) {
    p->wins += 1;
    sprintf(p->winsString, "%d", p->wins);

    int textWidth = MeasureText(p->winsString, winsFontSize);
    int padding = 50;
    switch (p->startLocation) {
        case TOPLEFT:
            p->winsStringX = padding * 1.5;
            p->winsStringY = padding;
            break;
        case BOTTOMLEFT:
            p->winsStringX = padding * 1.5;
            p->winsStringY = screenHeight - padding - winsFontSize;
            break;
        case TOPRIGHT:
            p->winsStringX = screenWidth - padding * 1.5 - textWidth;
            p->winsStringY = padding;
            break;
        case BOTTOMRIGHT:
            p->winsStringX = screenWidth - padding * 1.5 - textWidth;
            p->winsStringY = screenHeight - padding - winsFontSize;
            break;
        default:
            p->winsStringX = (screenWidth - textWidth) / 2;
            p->winsStringY = (screenHeight - winsFontSize) / 2;
    }
}

void DrawPlayerScore(struct Player *p) {
    DrawText(p->winsString, p->winsStringX, p->winsStringY, winsFontSize,
             ColorAlpha(ColorFromHSV(p->huePhase, 0.7, 1), 0.2));
}

bool IsPlayerShooting(struct Player *p) {
    if (GetTime() - p->lastShotTime > p->shotCooldownTime &&
        (p->shootingDirection.x != 0 || p->shootingDirection.y != 0)) {
        p->lastShotTime = GetTime();
        if (p->booletType == STRAIGHT) p->lastShotTime -= p->shotCooldownTime * 0.5;
        if (p->booletType == SWIRLY) p->lastShotTime -= p->shotCooldownTime * 0.25;
        else if (p->booletType == EXPLODING) p->lastShotTime += p->shotCooldownTime * 0.5;
        else if (p->booletType == HITSCAN) p->lastShotTime += p->shotCooldownTime;
        return true;
    }
    return false;
}
