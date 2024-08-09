//
// Created by igor on 2024.07.09..
//

#include "raylib.h"
#include "wall.h"
#include "../constants.h"

#ifndef DOORS_GAME_LEVEL_H
#define DOORS_GAME_LEVEL_H

struct Level {
    bool enabled;
    char name[16];
    struct Vector2 spawnLocations[playerCount];
    struct Wall walls[maxWallCount];
};

void LoadAllLevels(struct Level levels[], bool loadBorders);

#endif //DOORS_GAME_LEVEL_H
