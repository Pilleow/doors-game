//
// Created by igor on 2024.07.06..
//

#include "boolet.h"

void InitBooletDefaults(struct Boolet *b, struct Player *parent, float xStart, float yStart, float size, float xVelocity, float yVelocity,
                        unsigned char damage, unsigned short speed, BooletType btype, Color color) {
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
}

void DrawBoolet(struct Boolet *b) {
    DrawRectangle(b->rect.x, b->rect.y, b->rect.width, b->rect.height, b->color);
}

void ApplyBooletVelocity(struct Boolet *b) {
    float frameTime = GetFrameTime();
    if (b->velocity.x != 0) b->rect.x += b->velocity.x * b->speed * frameTime;
    if (b->velocity.y != 0) b->rect.y += b->velocity.y * b->speed * frameTime;
}
