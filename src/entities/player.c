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
    p->rect.height = 50;
    p->rect.width = 50;
    p->velocity.x = 0;
    p->velocity.y = 0;
    p->startLocation = atCorner;
    SetPlayerLocation(p, p->startLocation);
    p->shootingDirection.x = 0;
    p->shootingDirection.y = 0;
    p->maxHealth = 2;
    p->health = p->maxHealth;
    p->defaultSpeed = 400;
    p->totalGameWins = 0;
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
    p->lastTimeBlinked = -100;
    p->lastTimeTakenDamage = -100;
    p->wins = -1;
    AddWinToPlayer(p);
    switch (atCorner) {
        case TOPLEFT:
            p->huePhase = 0;
            break;
        case TOPRIGHT:
            p->huePhase = 120;
            break;
        case BOTTOMLEFT:
            p->huePhase = 60;
            break;
        case BOTTOMRIGHT:
            p->huePhase = 240;
            break;
    }
    p->lastShotTime = GetTime();
    p->shotCooldownTime = 0.5F;
    p->booletType = HITSCAN;
    p->booletAmplitude = 3;
    p->booletSize = 6;
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
    switch (atCorner) {
        case TOPLEFT:
            p->rect.x = 0.1F * screenWidth - p->rect.width / 2;
            p->rect.y = 0.1F * screenHeight - p->rect.height / 2;
            break;
        case TOPRIGHT:
            p->rect.x = 0.9F * screenWidth - p->rect.width / 2;
            p->rect.y = 0.1F * screenHeight - p->rect.height / 2;
            break;
        case BOTTOMLEFT:
            p->rect.x = 0.1F * screenWidth - p->rect.width / 2;
            p->rect.y = 0.9F * screenHeight - p->rect.height / 2;
            break;
        case BOTTOMRIGHT:
            p->rect.x = 0.9F * screenWidth - p->rect.width / 2;
            p->rect.y = 0.9F * screenHeight - p->rect.height / 2;
            break;
        default:
            p->rect.x = screenWidth / 2 - p->rect.width / 2;
            p->rect.y = screenHeight / 2 - p->rect.height / 2;
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
    float x, y;

    if (p->speed < p->defaultSpeed * 1.1) {
        p->velocity.x *= (p->friction < 0.97 ? p->friction : 0.97);
        p->velocity.y *= (p->friction < 0.97 ? p->friction : 0.97);
    }
    if (IsKeyDown(p->keyMoveUp)) p->velocity.y -= 1;
    if (IsKeyDown(p->keyMoveDown)) p->velocity.y += 1;
    if (IsKeyDown(p->keyMoveLeft)) p->velocity.x -= 1;
    if (IsKeyDown(p->keyMoveRight)) p->velocity.x += 1;

    x = GetGamepadAxisMovement(gamepadId, GAMEPAD_AXIS_LEFT_X);
    y = GetGamepadAxisMovement(gamepadId, GAMEPAD_AXIS_LEFT_Y);

    p->velocity.x = x == 0 ? p->velocity.x : x;
    p->velocity.y = y == 0 ? p->velocity.y : y;

    if (Vector2LengthSqr(p->velocity) > 1) {
        p->velocity = Vector2Normalize(p->velocity);
    }

    if (p->speed > p->defaultSpeed) {
        p->speed = p->defaultSpeed + (p->friction < 0.97 ? p->friction : 0.97) * (p->speed - p->defaultSpeed);
    }
        if (
            (IsKeyDown(p->keyDodge) ||
             IsGamepadButtonDown(gamepadId, GAMEPAD_BUTTON_LEFT_TRIGGER_2))
            && GetTime() - p->lastDodgeTime > p->dodgeCooldownTime
            ) {
        p->lastDodgeTime = GetTime();
        p->speed *= 4;
        PlaySound(sfxDash[rand() % sfxDashCount]);
    }

    p->shootingDirection.x = 0;
    p->shootingDirection.y = 0;

    if (IsKeyDown(p->keyShootUp)) p->shootingDirection.y -= 1;
    if (IsKeyDown(p->keyShootDown)) p->shootingDirection.y += 1;
    if (IsKeyDown(p->keyShootLeft)) p->shootingDirection.x -= 1;
    if (IsKeyDown(p->keyShootRight)) p->shootingDirection.x += 1;

    x = GetGamepadAxisMovement(gamepadId, GAMEPAD_AXIS_RIGHT_X);
    y = GetGamepadAxisMovement(gamepadId, GAMEPAD_AXIS_RIGHT_Y);

    p->shootingDirection.x = x == 0 ? p->shootingDirection.x : x;
    p->shootingDirection.y = y == 0 ? p->shootingDirection.y : y;

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
    p->lastTimeTakenDamage = GetTime();
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
    float s = (float) p->health / (float) p->maxHealth;
    Color c = ColorFromHSV(p->huePhase, s, 1);
    DrawRectangle(p->rect.x, p->rect.y, p->rect.width, p->rect.height, ColorFromHSV(p->huePhase, 1, 0.7));
    DrawRectangle(p->rect.x + pd, p->rect.y + pd, p->rect.width - 2 * pd, p->rect.height - 2 * pd,
                  c);

    int eyeSpacing = p->rect.width / 5;
    int eyeHeight = p->rect.height / 2.5;
    int eyeWidth = p->rect.width / 8;
    float eyeMovementFactor = 4;
    Vector2 eyeMovement = {
            (p->rect.x - p->pastPos[8].x) / 10,
            (p->rect.y - p->pastPos[8].y) / 10
    };
    if (Vector2LengthSqr(eyeMovement) > 1)
        eyeMovement = Vector2Normalize(eyeMovement);

    if (GetTime() - p->lastTimeTakenDamage > 0.5 && p->speed < 2 * p->defaultSpeed) {
        if (GetTime() - p->lastTimeBlinked < 0.1) {
            eyeHeight = eyeWidth;
            eyeWidth *= 1.7;
        } else if (rand() % 100 < GetTime() - p->lastTimeBlinked - 3) {
            eyeHeight = eyeWidth;
            eyeWidth *= 1.5;
            p->lastTimeBlinked = GetTime();
        }
        Rectangle leftEye = {
                p->rect.x + (p->rect.width - eyeSpacing) / 2 - eyeWidth + eyeMovementFactor * eyeMovement.x,
                p->rect.y + (p->rect.height - eyeHeight) / 2 + eyeMovementFactor * eyeMovement.y,
                eyeWidth, eyeHeight
        };
        Rectangle rightEye = {
                p->rect.x + (p->rect.width + eyeSpacing) / 2 + eyeMovementFactor * eyeMovement.x,
                p->rect.y + (p->rect.height - eyeHeight) / 2 + eyeMovementFactor * eyeMovement.y,
                eyeWidth, eyeHeight
        };
        DrawRectangleRec(leftEye, ColorFromHSV(p->huePhase, 1, 0.7));
        DrawRectangleRec(rightEye, ColorFromHSV(p->huePhase, 1, 0.7));
    } else {
        DrawLineEx(
                (Vector2) {
                        p->rect.x + (p->rect.width - 2 * eyeSpacing) / 2 - eyeWidth + eyeMovementFactor * eyeMovement.x,
                        p->rect.y + (p->rect.height - eyeHeight) / 2 + eyeMovementFactor * eyeMovement.y
                },
                (Vector2) {
                        p->rect.x + (p->rect.width + 2 * eyeSpacing) / 2 + eyeWidth + eyeMovementFactor * eyeMovement.x,
                        p->rect.y + (p->rect.height - eyeHeight) / 2 + eyeMovementFactor * eyeMovement.y + eyeHeight
                },
                eyeWidth,
                ColorFromHSV(p->huePhase, 1, 0.7)
        );
        DrawLineEx(
                (Vector2) {
                        p->rect.x + (p->rect.width + 2 * eyeSpacing) / 2 + eyeWidth + eyeMovementFactor * eyeMovement.x,
                        p->rect.y + (p->rect.height - eyeHeight) / 2 + eyeMovementFactor * eyeMovement.y
                },
                (Vector2) {
                        p->rect.x + (p->rect.width - 2 * eyeSpacing) / 2 - eyeWidth + eyeMovementFactor * eyeMovement.x,
                        p->rect.y + (p->rect.height - eyeHeight) / 2 + eyeMovementFactor * eyeMovement.y + eyeHeight
                },
                eyeWidth,
                ColorFromHSV(p->huePhase, 1, 0.7)
        );
        DrawRectangleRec(
                (Rectangle) {
                        p->rect.x + (p->rect.width - 0.5 * eyeSpacing) / 2 + eyeMovementFactor * eyeMovement.x,
                        p->rect.y + pd,
                        0.5 * eyeSpacing,
                        p->rect.height - 2 * pd
                },
                c
        );
    }
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

void SetWinToZeroPlayer(struct Player *p) {
    p->wins = -1;
    AddWinToPlayer(p);
}

void DrawPlayerScore(struct Player *p) {
    DrawText(p->winsString, p->winsStringX, p->winsStringY, winsFontSize,
             ColorAlpha(ColorFromHSV(p->huePhase, 0.7, 1), 0.6));
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
