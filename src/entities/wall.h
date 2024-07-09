//
// Created by igor on 2024.07.09..
//

#include "raylib.h"
#include "player.h"
#include "boolet.h"

#ifndef DOORS_GAME_WALL_H
#define DOORS_GAME_WALL_H

struct Wall {
    Rectangle rect;
    Color color;
    bool enabled;
};

bool CheckWallBooletCollisionAndFixPosition(struct Wall *w, struct Boolet *e);

bool CheckWallPlayerCollisionAndFixPosition(struct Wall *w, struct Player *e);

bool FixEntityPosition(struct Wall *w, Rectangle *rect);

void DrawWall(struct Wall *w);

#endif //DOORS_GAME_WALL_H
