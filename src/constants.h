//
// Created by igor on 2024.07.06..
//

#include <stdbool.h>
#include <math.h>

#ifndef RAYLIB_GAME_TEMPLATE_CONSTANTS_H
#define RAYLIB_GAME_TEMPLATE_CONSTANTS_H

#define playerEffectCapacityAndLifespan 2
#define recoilDodgeDivisionLimit 30
#define pastPlayerPositionsCount 16
#define maxBooletsOnMap 128
#define sfxDoorOpenCount 3
#define winsFontSize 96
#define doorFontSize 48
#define maxWallCount 128
#define sfxShootCount 7
#define sfxDeadCount 1
#define bgMusicCount 8
#define sfxDashCount 5
#define playerCount 4
#define sfxHitCount 1
#define levelCount 17

#define hueRotationSpeedOnWinMultiplier 90
#define hueRotationSpeedOnDeathMultiplier 60
#define hueRotationSpeedOnHitMultiplier 30

static bool showFPS = false;

static int playersPlaying = 2;
static int winsNeededToWinGame = 10;

static const float recoilScale = 0.7;
static const float dodgeSpeedMultiplier = 4;
static float playerEffectMultiplier = 1.5;
static float defaultHueRotationSpeed = 10;
extern float hueRotationSpeed;
extern float hueRotationTimer;

static int finalWidth = 1920;
static int finalHeight = 1080;
static int screenWidth = 1920;
static int screenHeight = 1080;

static float masterVolume = 0.0f;
static float bgMusicVolume = 0.5f;
static float sfxDoorOpenVolume = 0.5f;
static float sfxShootVolume = 0.5f;
static float sfxDeadVolume = 0.5f;
static float sfxDashVolume = 0.5f;
static float sfxHitVolume = 0.5f;

enum InputState {
    GAMEPAD_ONLY,
    MIXED,
    KEYBOARD_ONLY
};

static enum InputState inputState = MIXED;

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
    CHOOSEDOOR,
    TRANSITION
} GameState;

typedef enum {
    STRAIGHT,
    SWIRLY,
    TIMEBENDING,
    EXPLODING,
    BOUNCING,
    HITSCAN,
} BooletType;
static const int booletTypeCount = 6;   // this counts only implemented bullet types

#endif //RAYLIB_GAME_TEMPLATE_CONSTANTS_H
