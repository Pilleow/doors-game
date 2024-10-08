//
// Created by igor on 2024.07.06..
//

#include "raylib.h"
#include "player.h"
#include "../constants.h"

#ifndef RAYLIB_GAME_TEMPLATE_BOOLET_H
#define RAYLIB_GAME_TEMPLATE_BOOLET_H

struct Boolet {
    Rectangle rect;
    Vector2 velocity;
    unsigned char damage;
    int speed;
    int startingSpeed;
    bool enabled;
    BooletType type;
    struct Player* parent;
    Color color;
    int timeCreated;
    int amplitude;
    float decayTimeLeft;
    float glowAlpha;
};

void InitBooletDefaults(struct Boolet *b, struct Player *parent, float xStart, float yStart, float size, float xVelocity, float yVelocity,
                        unsigned char damage, unsigned short speed, BooletType btype, Color color, int amplitude, float decayTimeLeft);

void ExplodeBoolet(struct Boolet *b, int *nextBooletIndex, struct Boolet boolets[], BooletType type);

float GetBooletRecoilModifier(BooletType bT);

void MoveBulletBackOneStep(struct Boolet *b);

void DrawBoolet(struct Boolet *b);

void ApplyBooletVelocity(struct Boolet *b);

#endif //RAYLIB_GAME_TEMPLATE_BOOLET_H
