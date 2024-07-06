//
// Created by igor on 2024.07.06..
//

#ifndef RAYLIB_GAME_TEMPLATE_CONSTANTS_H
#define RAYLIB_GAME_TEMPLATE_CONSTANTS_H

#define maxBooletsOnMap 512
#define playerCount 4
#define playerEffectCapacityAndLifespan 4

#define debugMode true

static int playersPlaying = 2;
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

typedef enum {
    STRAIGHT,
    SWIRLY,
    EXPLODING
} BooletType;

#endif //RAYLIB_GAME_TEMPLATE_CONSTANTS_H
