//
// Created by igor on 2024.07.06..
//

#include "boolet.h"

#include <math.h>

void InitBooletDefaults(struct Boolet *b, struct Player *parent, float xStart, float yStart, float size, float xVelocity, float yVelocity,
                        unsigned char damage, unsigned short speed, BooletType btype, Color color, int amplitude) {
    if (btype == EXPLODING) {
        size *= 2;
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
    b->enabled = true;
    b->type = btype;
    b->color = color;
    b->amplitude = amplitude;
    b->timeCreated = GetTime();
    b->decayTimeLeft = 1.3;
}

void ExplodeBoolet(struct Boolet *b, int *nextBooletIndex, struct Boolet boolets[], BooletType type) {
    float x, y;
    for (int j = 0; j < 8; ++j) {
        switch (j) {
            case 0: // left
                x = -1; y = 0;
                break;
            case 1: // left up
                x = -0.707; y = -0.707;
                break;
            case 2: // up
                x = 0; y = -1;
                break;
            case 3: // right up
                x = 0.707; y = -0.707;
                break;
            case 4: // right
                x = 1; y = 0;
                break;
            case 5: // right down
                x = 0.707; y = 0.707;
                break;
            case 6: // down
                x = 0; y = 1;
                break;
            case 7: // down left
                x = -0.707; y = 0.707;
                break;
            default: // default: down
                x = 0; y = 1;
        }
        InitBooletDefaults(
                &boolets[*nextBooletIndex], b->parent,
                b->rect.x + b->rect.width / 2, b->rect.y + b->rect.height / 2, 5,
                x, y,
                1, 400, type, b->color, b->amplitude
        );
        boolets[*nextBooletIndex].decayTimeLeft = 0.5;
        (*nextBooletIndex)++;
        *nextBooletIndex %= maxBooletsOnMap;
    }
    b->enabled = false;
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

        float swayX, swayY;
    switch (b->type) {
        case EXPLODING:
            b->speed *= 0.97;
            if (b->velocity.x != 0) b->rect.x += b->velocity.x * b->speed * frameTime * 1.25;
            if (b->velocity.y != 0) b->rect.y += b->velocity.y * b->speed * frameTime * 1.25;
            break;
        case SWIRLY:
            swayX = cosf((GetTime() - b->timeCreated) * (b->amplitude + 1));
            swayY = sinf((GetTime() - b->timeCreated) * (b->amplitude + 1));
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
            swayX = sinf((GetTime() - b->timeCreated) * (b->amplitude - 2));
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
        default: // default == STRAIGHT
            if (b->velocity.x != 0) b->rect.x += b->velocity.x * b->speed * frameTime * 1.25;
            if (b->velocity.y != 0) b->rect.y += b->velocity.y * b->speed * frameTime * 1.25;
    }
}
