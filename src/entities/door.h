//
// Created by igor on 2024.07.06..
//

#include "../constants.h"
#include "raylib.h"

#ifndef RAYLIB_GAME_TEMPLATE_DOOR_H
#define RAYLIB_GAME_TEMPLATE_DOOR_H

struct Door {
    Location location;
    float timeSpawned;
    float animationOpenTime;
    Color color;

};

#endif //RAYLIB_GAME_TEMPLATE_DOOR_H
