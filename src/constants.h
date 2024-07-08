//
// Created by igor on 2024.07.06..
//

#ifndef RAYLIB_GAME_TEMPLATE_CONSTANTS_H
#define RAYLIB_GAME_TEMPLATE_CONSTANTS_H

#define playerEffectCapacityAndLifespan 3
#define pastPlayerPositionsCount 32
#define maxBooletsOnMap 512
#define doorFontSize 64
#define winsFontSize 128
#define playerCount 4

static int showFPS = 1;

static int playersPlaying = 2;
static int screenWidth = 1920;
static int screenHeight = 1080;

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
    EXPLODING,
    TIMEBENDING,
    HITSCAN
} BooletType;
static const int booletTypeCount = 5;

#endif //RAYLIB_GAME_TEMPLATE_CONSTANTS_H
