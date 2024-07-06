#include "raylib.h"
#include "raymath.h"

#include "player.h"
#include "../constants.h"

void InitPlayerDefaults(
        struct Player *p, Location atCorner,
        KeyboardKey up, KeyboardKey down,
        KeyboardKey left, KeyboardKey right,
        KeyboardKey upShoot, KeyboardKey downShoot,
        KeyboardKey leftShoot, KeyboardKey rightShoot
) {
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
    p->rect.height = 30;
    p->rect.width = 30;
    p->velocity.x = 0;
    p->velocity.y = 0;
    p->shootingDirection.x = 0;
    p->shootingDirection.y = 0;
    p->maxHealth = 5;
    p->health = p->maxHealth;
    p->speed = 300;
    p->keyMoveUp = up;
    p->keyMoveDown = down;
    p->keyMoveLeft = left;
    p->keyMoveRight = right;
    p->keyShootUp = upShoot;
    p->keyShootDown = downShoot;
    p->keyShootLeft = leftShoot;
    p->keyShootRight = rightShoot;
    p->isDead = false;
    p->huePhase = atCorner * 60;
    p->lastShotTime = 0;
    p->shotCooldownTime = 0.5F;
    p->friction = 0.9F;
    p->bulletSpeed = 500;
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
                  p->rect.height * s,ColorFromHSV((int)(30 * GetTime()) % 360, 1, .5));
}

bool IsPlayerShooting(struct Player *p) {
    if (GetTime() - p->lastShotTime > p->shotCooldownTime &&
        (p->shootingDirection.x != 0 || p->shootingDirection.y != 0)) {
        p->lastShotTime = GetTime();
        return true;
    }
    return false;
}
