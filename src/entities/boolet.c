//
// Created by igor on 2024.07.06..
//

#include "boolet.h"

#include <math.h>

void InitBooletDefaults(struct Boolet *b, struct Player *parent, float xStart, float yStart, float size, float xVelocity, float yVelocity,
                        unsigned char damage, unsigned short speed, BooletType btype, Color color, int amplitude) {
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
}

void DrawBoolet(struct Boolet *b) {
    DrawRectangle(b->rect.x, b->rect.y, b->rect.width, b->rect.height, b->color);
}

void ApplyBooletVelocity(struct Boolet *b) {
    float frameTime = GetFrameTime();
    float swayX, swayY;
    switch (b->type) {
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
