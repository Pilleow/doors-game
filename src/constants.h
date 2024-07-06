//
// Created by igor on 2024.07.06..
//

#ifndef RAYLIB_GAME_TEMPLATE_CONSTANTS_H
#define RAYLIB_GAME_TEMPLATE_CONSTANTS_H

#define maxBooletsOnMap 512
#define playerCount 4

static const int screenWidth = 800;
static const int screenHeight = 450;

typedef enum {
    TOPLEFT,
    TOP,
    TOPRIGHT,
    LEFT,
    CENTER,
    RIGHT,
    BOTTOMLEFT,
    BOTTOM,
    BOTTOMRIGHT
} Location;

typedef enum {
    FIGHT,
    CHOOSEDOOR
} GameState;

#endif //RAYLIB_GAME_TEMPLATE_CONSTANTS_H
