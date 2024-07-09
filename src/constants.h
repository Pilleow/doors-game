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
#define sfxDoorOpenCount 3
#define sfxShootCount 3
#define sfxDeadCount 1
#define bgMusicCount 8
#define sfxDashCount 5
#define sfxHitCount 1
#define playerCount 4

static bool useGamepads = true;
static bool showFPS = true;

static int playersPlaying = 2;

static int finalWidth = 1920;
static int finalHeight = 1080;
static int screenWidth = 1920;
static int screenHeight = 1080;

static float masterVolume = 1.0f;
static float bgMusicVolume = 0.5f;
static float sfxDoorOpenVolume = 0.5f;
static float sfxShootVolume = 0.5f;
static float sfxDeadVolume = 0.5f;
static float sfxDashVolume = 0.5f;
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
    TIMEBENDING,
    EXPLODING,
    HITSCAN,                // todo implement, hitscan (too op? maybe lower fire rate?)
    BOUNCING,               // todo implement, projectile bounces one or more times from the edge
    ROTATING,               // todo implement, bullets rotate outwards in a spiral from the point where they spawned
    INVERSE_ROTATING,       // todo implement, same as above but the other way
} BooletType;
static const int booletTypeCount = 4;   // this counts only implemented bullet types

#endif //RAYLIB_GAME_TEMPLATE_CONSTANTS_H
