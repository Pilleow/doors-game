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
    int playerEffect;
    char playerEffectString[32];
    int playerEffectStringPositionX;
    int playerEffectStringPositionY;
    int playerEffectsStringWidth;
    bool isDebuff;
    Rectangle finalRect;
};

float _functionDrawDoorCalculate(struct Door *d, float x);

void InitDoorsWithRandomEffect(struct Door *d);

void DrawDoor(struct Door *d);

#endif //RAYLIB_GAME_TEMPLATE_DOOR_H
