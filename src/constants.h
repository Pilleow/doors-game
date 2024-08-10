//
// Created by igor on 2024.07.06..
//

#include <stdbool.h>
#include <math.h>
#include "raylib.h"

#ifndef RAYLIB_GAME_TEMPLATE_CONSTANTS_H
#define RAYLIB_GAME_TEMPLATE_CONSTANTS_H

#define playerEffectCapacityAndLifespan 3
#define recoilDodgeDivisionLimit 30
#define pastPlayerPositionsCount 16
#define mainMenuButtonCount 24
#define maxBooletsOnMap 64
#define sfxDoorOpenCount 3
#define maxPickupItems 4
#define winsFontSize 96
#define maxParticles 64
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

extern bool showFPS;
extern bool randomizeEffectsEveryRound;
extern bool playersKeepWeaponsBetweenRounds;

static int playersPlaying = 4;
extern int winsNeededToWinGame;

static const float dodgeSpeedMultiplier = 4;
static float defaultHueRotationSpeed = 10;

extern float playerEffectMultiplier;
extern float recoilScale;
extern float hueRotationSpeed;
extern float hueRotationTimer;

static int finalWidth = 1920;
static int finalHeight = 1080;
static int screenWidth = 1920;
static int screenHeight = 1080;

extern float masterVolume;
extern float bgMusicVolume;
extern float sfxDoorOpenVolume;
extern float sfxShootVolume;
extern float sfxDeadVolume;
extern float sfxDashVolume;
extern float sfxHitVolume;

enum InputState {
    GAMEPAD_ONLY,
    MIXED,
    KEYBOARD_ONLY
};

extern enum InputState inputState;

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
    SHOTGUN,
    COPYMOVE,
    BOOMERANG,
    INVERTED,       // turned off
    RANDOM          // turned off
} BooletType;
static const int booletTypeCount = 9;   // this counts only implemented bullet types
extern Texture2D booletIconSprites[9];

typedef enum {
    BOOLETSWAP,
} PickupItemType;

#endif //RAYLIB_GAME_TEMPLATE_CONSTANTS_H
