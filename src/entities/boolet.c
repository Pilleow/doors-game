//
// Created by igor on 2024.07.06..
//

#include "boolet.h"
#include "raymath.h"

#include <math.h>
#include <limits.h>

void InitBooletDefaults(struct Boolet *b, struct Player *parent, float xStart, float yStart, float size,
                        float xVelocity, float yVelocity, unsigned char damage, unsigned short speed, BooletType btype,
                        Color color, int amplitude, float decayTimeLeft) {
    if (btype == SWIRLY) {
        size *= 1.5;
    } else if (btype == EXPLODING) {
        size *= 2;
        speed *= 1.5;
    } else if (btype == BOUNCING) {
        decayTimeLeft *= 1.3;
    } else if (btype == TIMEBENDING) {
        decayTimeLeft *= 2;
        speed *= 0.9;
    } else if (btype == HITSCAN) {
        speed = 0;
    } else if (btype == SHOTGUN) {
        decayTimeLeft *= 0.4;
    } else if (btype == COPYMOVE) {
        decayTimeLeft *= 3;
        speed *= 0.9;
        size *= 2;
    } else if (btype == INVERTED) {
        size *= 1.5;
    } else if (btype == BOOMERANG) {
        decayTimeLeft = (float)LONG_MAX;
        speed *= 1.5;
    }
    b->parent = parent;
    b->rect.x = xStart;
    b->rect.y = yStart;
    b->rect.width = size;
    b->rect.height = size;
    b->damage = damage;
    b->velocity.x = xVelocity;
    b->velocity.y = yVelocity;
    b->speed = speed;
    b->startingSpeed = speed;
    b->enabled = true;
    b->type = btype;
    b->color = color;
    b->amplitude = amplitude;
    b->timeCreated = GetTime();
    b->decayTimeLeft = decayTimeLeft;
}

void ExplodeBoolet(struct Boolet *b, int *nextBooletIndex, struct Boolet boolets[], BooletType type) {
    float x, y;
    for (int j = 0; j < 8; ++j) {
        switch (j) {
            case 0: // left
                x = -1;
                y = 0;
                break;
            case 1: // left up
                x = -0.707;
                y = -0.707;
                break;
            case 2: // up
                x = 0;
                y = -1;
                break;
            case 3: // right up
                x = 0.707;
                y = -0.707;
                break;
            case 4: // right
                x = 1;
                y = 0;
                break;
            case 5: // right down
                x = 0.707;
                y = 0.707;
                break;
            case 6: // down
                x = 0;
                y = 1;
                break;
            case 7: // down left
                x = -0.707;
                y = 0.707;
                break;
            default: // default: down
                x = 0;
                y = 1;
        }
        InitBooletDefaults(
                &boolets[*nextBooletIndex], b->parent,
                b->rect.x + b->rect.width / 2, b->rect.y + b->rect.height / 2, 5,
                x, y,
                1, 400, type, b->color, b->amplitude, 0.5
        );
        (*nextBooletIndex)++;
        *nextBooletIndex %= maxBooletsOnMap;
    }
    b->enabled = false;
}

void MoveBulletBackOneStep(struct Boolet *b) {
    b->velocity.x *= -1;
    b->velocity.y *= -1;
    ApplyBooletVelocity(b);
    b->velocity.x *= -1;
    b->velocity.y *= -1;
}

void DrawBoolet(struct Boolet *b) {
    DrawRectangle(b->rect.x, b->rect.y, b->rect.width, b->rect.height, b->color);
}

void ApplyBooletVelocity(struct Boolet *b) {
    float frameTime = GetFrameTime();

    b->decayTimeLeft -= frameTime;
    if (b->decayTimeLeft < 0) {
        b->enabled = false;
        return;
    } else if (b->decayTimeLeft < 0.2) {
        b->rect.width *= 0.9;
        b->rect.height *= 0.9;
    }

    Vector2Normalize(b->velocity);

    float swayX, swayY;
    switch (b->type) {
        case HITSCAN:
            break;
        case COPYMOVE:
            b->velocity.x += b->parent->velocity.x * frameTime * b->parent->booletAmplitude * 2;
            b->velocity.y += b->parent->velocity.y * frameTime * b->parent->booletAmplitude * 2;
            b->velocity = Vector2Normalize(b->velocity);
            if (b->velocity.x != 0) b->rect.x += b->velocity.x * b->speed * frameTime * 1.25;
            if (b->velocity.y != 0) b->rect.y += b->velocity.y * b->speed * frameTime * 1.25;
            break;
        case EXPLODING:
            b->speed *= 0.97;
            if (b->velocity.x != 0) b->rect.x += b->velocity.x * b->speed * frameTime * 1.25;
            if (b->velocity.y != 0) b->rect.y += b->velocity.y * b->speed * frameTime * 1.25;
            break;
        case SWIRLY:
            swayX = cosf((GetTime() - b->timeCreated) * (b->amplitude + 10));
            swayY = sinf((GetTime() - b->timeCreated) * (b->amplitude + 10));
            if (b->velocity.x != 0) {
                b->rect.x += b->velocity.x * b->speed * frameTime;
                b->rect.y += b->amplitude * swayX * b->velocity.x * 60 * frameTime;
            }
            if (b->velocity.y != 0) {
                b->rect.y += b->velocity.y * b->speed * frameTime;
                b->rect.x += b->amplitude * swayY * b->velocity.y * 60 * frameTime;
            }
            break;
        case TIMEBENDING:
            swayX = sinf((GetTime() - b->timeCreated) * (b->amplitude + 5));
            swayY = swayX;
            if (b->velocity.x != 0) {
                b->rect.x += b->velocity.x * b->speed * frameTime;
                b->rect.x += (b->amplitude + 2) * swayX * b->velocity.x * 60 * frameTime;
            }
            if (b->velocity.y != 0) {
                b->rect.y += b->velocity.y * b->speed * frameTime;
                b->rect.y += (b->amplitude + 2) * swayY * b->velocity.y * 60 * frameTime;
            }
            break;
        case BOOMERANG:
            if (b->speed >= -b->startingSpeed) b->speed -= b->parent->booletAmplitude * 5;
            if (b->speed < -b->startingSpeed * 0.7 && b->decayTimeLeft > 0.3) b->decayTimeLeft = 0.3;
            if (b->velocity.x != 0) b->rect.x += b->velocity.x * b->speed * frameTime * 1.25;
            if (b->velocity.y != 0) b->rect.y += b->velocity.y * b->speed * frameTime * 1.25;
            break;
        case INVERTED:
            if (b->velocity.x != 0) b->rect.x -= b->velocity.x * b->speed * frameTime * 1.25;
            if (b->velocity.y != 0) b->rect.y -= b->velocity.y * b->speed * frameTime * 1.25;
            break;
        default: // default == STRAIGHT
            if (b->velocity.x != 0) b->rect.x += b->velocity.x * b->speed * frameTime * 1.25;
            if (b->velocity.y != 0) b->rect.y += b->velocity.y * b->speed * frameTime * 1.25;
    }
}

float GetBooletRecoilModifier(BooletType bT) {
    float mult = 1;

    switch (bT) {
        case INVERTED:
            mult = -1;
            break;
        case TIMEBENDING:
            mult = 0.75;
            break;
        case SWIRLY:
            mult = 0.75;
            break;
        case BOOMERANG:
            mult = 0.75;
            break;
        case STRAIGHT:
            mult = 1;
            break;
        case COPYMOVE:
            mult = 1.5;
            break;
        case BOUNCING:
            mult = 1.5;
            break;
        case SHOTGUN:
            mult = 1.75;
            break;
        case HITSCAN:
            mult = 2;
            break;
        case EXPLODING:
            mult = 2;
            break;
        default:
            mult = 1;
    }

    return mult * recoilScale;
}
