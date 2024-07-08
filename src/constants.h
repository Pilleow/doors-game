//
// Created by igor on 2024.07.06..
//

#include <stdbool.h>

#ifndef RAYLIB_GAME_TEMPLATE_CONSTANTS_H
#define RAYLIB_GAME_TEMPLATE_CONSTANTS_H

#define playerEffectCapacityAndLifespan 3
#define maxBooletsOnMap 512
#define doorFontSize 64
#define winsFontSize 128

#define pastPlayerPositionsCount 32
#define sfxDeadCount 1
#define sfxHitCount 1
#define sfxShootCount 3
#define playerCount 4
#define bgMusicCount 1

static bool useGamepads = true;
static bool showFPS = false;

static int playersPlaying = 2;

static int finalWidth = 1920;
static int finalHeight = 1080;
static int screenWidth = 1920;
static int screenHeight = 1080;

static float masterVolume = 0.0f;
static float bgMusicVolume = 0.5f;
static float sfxShootVolume = 0.5f;
static float sfxDeadVolume = 0.5f;
static float sfxHitVolume = 0.5f;

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
