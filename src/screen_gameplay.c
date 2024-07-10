#include "raylib.h"
#include "screens.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "constants.h"
#include "entities/wall.h"
#include "entities/door.h"
#include "entities/level.h"
#include "entities/player.h"
#include "entities/boolet.h"
#include "entities/playerEffect.h"

// local variable declaration below ------------------------------------------------------------------------------------

static GameState gameState = FIGHT;
static Rectangle bgRect1;
static Rectangle bgRect2;
static Vector2 center;
struct Player players[playerCount];
struct Boolet boolets[maxBooletsOnMap];
struct Level levels[levelCount];
struct Door doors[4];
float hueRotationSpeed = 40;
float hueRotationTimer = 0;
static int currentLevelIndex = 2;
static int nextBooletIndex = 0;
static char fpsString[16];
static char playersCurrentlyPlaying;
bool gotoLevelEditor = false;

// functions definition below ------------------------------------------------------------------------------------------

// this function initializes the gameplay screen
void InitGameplayScreen(void) {
    gotoLevelEditor = false;
    LoadAllLevels(levels);

    bgRect1.x = screenWidth / 2 + screenWidth * 0.03;
    bgRect1.y = screenHeight / 2 + screenHeight * 0.03;
    bgRect1.width = screenWidth * 0.94;
    bgRect1.height = screenHeight * 0.94;

    bgRect2.x = screenWidth / 2 + screenWidth * 0.06;
    bgRect2.y = screenHeight / 2 + screenHeight * 0.06;
    bgRect2.width = screenWidth * 0.88;
    bgRect2.height = screenHeight * 0.88;

    center.x = screenWidth / 2;
    center.y = screenHeight / 2;

    struct Player player1, player2, player3, player4;
    InitPlayerDefaults(&player1,
                       TOPLEFT,
                       KEY_W, KEY_S, KEY_A, KEY_D,
                       KEY_W, KEY_S, KEY_A, KEY_D,
                       KEY_LEFT_ALT
    );
    players[0] = player1;
    InitPlayerDefaults(&player2,
                       BOTTOMRIGHT,
                       KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT,
                       KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT,
                       KEY_RIGHT_CONTROL
    );
    players[1] = player2;
    InitPlayerDefaults(&player3,
                       BOTTOMLEFT, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL,
                       KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL,
                       KEY_NULL
    );
    players[2] = player3;
    InitPlayerDefaults(&player4,
                       TOPRIGHT,
                       KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL,
                       KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL,
                       KEY_NULL
    );
    players[3] = player4;

    if (useGamepads) {
        playersPlaying = 0;
        for (int i = 0; i < playerCount; ++i) players[i].isPlaying = false;
    }
    if (playersPlaying < 3) players[2].isPlaying = false;
    if (playersPlaying < 4) players[3].isPlaying = false;

    for (int i = 0; i < maxBooletsOnMap; ++i) boolets[i].enabled = false;

    doors[0].location = LEFT;
    doors[1].location = RIGHT;
    doors[2].location = TOP;
    doors[3].location = BOTTOM;
    for (int i = 0; i < 4; ++i) InitDoorsWithRandomEffect(&doors[i]);
}

// this function resets the players and map for a new round of the game
void resetLevel() {
    char atCornerPhase = rand() % 4;
    Location possibleSpawnLocations[] = {
            TOPLEFT, TOPRIGHT,
            BOTTOMLEFT, BOTTOMRIGHT
    };
    for (int i = 0; i < playerCount; ++i) {
        SetPlayerLocation(&players[i], possibleSpawnLocations[(i + atCornerPhase) % 4]);
        players[i].isDead = false;
        players[i].velocity.x = 0;
        players[i].velocity.y = 0;
        players[i].lastDodgeTime = GetTime();
        players[i].lastShotTime = GetTime();
        players[i].health = players[i].maxHealth;
        players[i].booletType = rand() % booletTypeCount;
        for (int j = 0; j < pastPlayerPositionsCount; ++j) {
            players[i].pastPos[j].x = players[i].rect.x;
            players[i].pastPos[j].y = players[i].rect.y;
        }
    }
    if (playersPlaying < 3) players[2].isPlaying = false;
    if (playersPlaying < 4) players[3].isPlaying = false;
    for (int i = 0; i < 4; ++i) {
        InitDoorsWithRandomEffect(&doors[i]);
    }
    currentLevelIndex++;
    currentLevelIndex %= levelCount;
}

// this function returns if ANY PART of the rectangle is out of bounds
// returns -1 if not out of bounds or out of bounds LOCATION if out of bounds
int isOutOfWindowBounds(Rectangle r) {
    if (r.y + r.height / 2 < 0) return TOP;
    if (r.x + r.width < 0) return LEFT;
    if (r.x + r.width > (float) screenWidth) return RIGHT;
    if (r.y + r.height / 2 > (float) screenHeight) return BOTTOM;
    return -1;
}

// this function updates the game when gameState == FIGHT
void updateGameplayScreenDuringFight() {
    // look for gamepads -----------------------------------------------------------------------------------------------
    if (useGamepads && playersPlaying < playerCount) {
        for (int gamepadId = 0; gamepadId < playerCount; ++gamepadId) {
            if (!IsGamepadAvailable(gamepadId)) {
                if (players[gamepadId].isPlaying == false) continue;
                else {
                    players[gamepadId].isPlaying = false;
                    players[gamepadId].isDead = true;
                    playersPlaying--;
                }
            }
            if (players[gamepadId].isPlaying == false) {
                players[gamepadId].isPlaying = true;
                players[gamepadId].isDead = false;
                playersPlaying++;
            }
        }
    }

    // player update ---------------------------------------------------------------------------------------------------
    char playersAlive = 0;
    playersCurrentlyPlaying = 0;

    for (int i = 0; i < 4; ++i) {
        if (players[i].isPlaying) playersCurrentlyPlaying++;
        else continue;
        if (!players[i].isDead) playersAlive++;
        else continue;
        struct Player *p = &players[i];
        ProcessPlayerInput(p, i);
        ApplyPlayerVelocity(p);
        for (int j = 0; j < maxWallCount; ++j)
            CheckWallPlayerCollisionAndFixPosition(&levels[currentLevelIndex].walls[j], p);
        if (IsPlayerShooting(p)) {
            InitBooletDefaults(
                    &boolets[nextBooletIndex++], p,
                    p->rect.x + p->rect.width / 2, p->rect.y + p->rect.height / 2,
                    5, p->shootingDirection.x, p->shootingDirection.y, 1, p->bulletSpeed,
                    p->booletType, ColorFromHSV(p->huePhase, 1, 1), p->booletAmplitude, p->booletDecayTimeLeft
            );
            nextBooletIndex %= maxBooletsOnMap;
            PlaySound(sfxShoot[p->sfxShootSoundIndex++]);
            p->sfxShootSoundIndex %= sfxShootCount;
        }
    }

    // if there is 1 or 0 players alive --------------------------------------------------------------------------------
    if (playersAlive <= 1 && playersCurrentlyPlaying >= 2) {
        struct Player *alivePlayer;
        for (int k = 0; k < 4; ++k) {
            if (!players[k].isDead && players[k].isPlaying) {
                alivePlayer = &players[k];
                break;
            }
        }
        if (playersAlive == 0) resetLevel();
        else if (playersAlive == 1) {
            AddWinToPlayer(alivePlayer);
            gameState = CHOOSEDOOR;
            hueRotationSpeed = defaultHueRotationSpeed * hueRotationSpeedOnWinMultiplier;
            PlaySound(sfxDoorOpen[rand() % sfxDoorOpenCount]);
            for (int k = 0; k < 4; ++k) doors[k].timeSpawned = GetTime();
        }
    }

    // boolet update ---------------------------------------------------------------------------------------------------
    for (int i = 0; i < maxBooletsOnMap; ++i) {
        if (!boolets[i].enabled) continue;
        int oob = isOutOfWindowBounds(boolets[i].rect);
        if (oob != -1) {
            if (boolets[i].type == BOUNCING) {
                switch (oob) {
                    case TOP:
                    case BOTTOM:
                        boolets[i].velocity.y *= -1;
                        break;
                    case LEFT:
                    case RIGHT:
                        boolets[i].velocity.x *= -1;
                        break;
                    default:
                        boolets[i].enabled = false;
                }
            } else if (boolets[i].type == EXPLODING) {
                MoveBulletBackOneStep(&boolets[i]);
                ExplodeBoolet(&boolets[i], &nextBooletIndex, boolets, STRAIGHT);
            } else boolets[i].enabled = false;
        }
        ApplyBooletVelocity(&boolets[i]);
        for (int j = 0; j < maxWallCount; ++j)
            if (CheckWallBooletCollisionAndFixPosition(&levels[currentLevelIndex].walls[j], &boolets[i])) {
                if (boolets[i].type == EXPLODING) {
                    MoveBulletBackOneStep(&boolets[i]);
                    ExplodeBoolet(&boolets[i], &nextBooletIndex, boolets, STRAIGHT);
                }
            }
        if (boolets[i].type == EXPLODING && boolets[i].speed < 300) {
            ExplodeBoolet(&boolets[i], &nextBooletIndex, boolets, STRAIGHT);
        }
        for (int j = 0; j < 4; ++j) {
            if (!players[j].isDead && players[j].isPlaying && &players[j] != boolets[i].parent &&
                CheckCollisionRecs(boolets[i].rect, players[j].rect)) {
                if (boolets[i].type == EXPLODING) {
                    MoveBulletBackOneStep(&boolets[i]);
                    ExplodeBoolet(&boolets[i], &nextBooletIndex, boolets, STRAIGHT);
                } else {
                    ApplyDamageToPlayer(&players[j], boolets[i].damage);
                    if (players[j].isDead) {
                        hueRotationSpeed = defaultHueRotationSpeed * hueRotationSpeedOnDeathMultiplier;
                        PlaySound(sfxDead[rand() % sfxDeadCount]);
                    } else {
                        hueRotationSpeed = defaultHueRotationSpeed * hueRotationSpeedOnHitMultiplier;
                        PlaySound(sfxHit[rand() % sfxHitCount]);
                    }
                    boolets[i].enabled = false;
                }
            }
        }
    }
}

// this function updates the game when gameState == CHOOSEDOOR
void updateGameplayScreenDuringChooseDoor() {
    // player update ---------------------------------------------------------------------------------------------------
    for (int i = 0; i < 4; ++i) {
        if (players[i].isDead || !players[i].isPlaying) continue;
        ProcessPlayerInput(&players[i], i);
        ApplyPlayerVelocity(&players[i]);
        for (int j = 0; j < maxWallCount; ++j)
            CheckWallPlayerCollisionAndFixPosition(&levels[currentLevelIndex].walls[j], &players[i]);
        for (int j = 0; j < 4; ++j) {
            if (CheckCollisionRecs(doors[j].finalRect, players[i].rect)) {
                if (doors[j].playerEffect == CLEAR_ALL_EFFECTS)
                    for (int k = 0; k < playerCount; ++k) ClearPlayerOfEffects(&players[k]);
                else if (doors[j].playerEffect == RANDOM_EFFECT_TO_EVERYONE)
                    for (int k = 0; k < playerCount; ++k) AssignEffectToPlayer(rand() % playerEffectCount, &players[k]);
                else if (doors[j].isDebuff) AssignEffectToPlayer(doors[j].playerEffect, &players[i]);
                else {
                    int r = i;
                    while (r == i) r = rand() % playersPlaying;
                    AssignEffectToPlayer(doors[j].playerEffect, &players[r]);
                }
                for (int k = 0; k < maxBooletsOnMap; ++k) boolets[k].enabled = false;

                resetLevel();
                gameState = FIGHT;
            }
        }
    }

    // boolet update ---------------------------------------------------------------------------------------------------
    for (int i = 0; i < maxBooletsOnMap; ++i) {
        if (!boolets[i].enabled) continue;
        if (isOutOfWindowBounds(boolets[i].rect)) boolets[i].enabled = false;
        ApplyBooletVelocity(&boolets[i]);
        for (int j = 0; j < maxWallCount; ++j)
            if (CheckWallBooletCollisionAndFixPosition(&levels[currentLevelIndex].walls[j], &boolets[i])) {
                if (boolets[i].type == EXPLODING) {
                    MoveBulletBackOneStep(&boolets[i]);
                    ExplodeBoolet(&boolets[i], &nextBooletIndex, boolets, STRAIGHT);
                }
            }
    }
}

// this function updates the game regardless of current gameState
void UpdateGameplayScreen(void) {
    hueRotationTimer += GetFrameTime() * hueRotationSpeed;
    if (hueRotationSpeed > defaultHueRotationSpeed) {
        hueRotationSpeed = defaultHueRotationSpeed + 0.98 * (hueRotationSpeed - defaultHueRotationSpeed);
    }

    // call function based on current gameState
    switch (gameState) {
        case FIGHT:
            updateGameplayScreenDuringFight();
            break;
        case CHOOSEDOOR:
            updateGameplayScreenDuringChooseDoor();
            break;
    }

    // print debug message on button press
    if (IsKeyPressed(KEY_F1)) {
        printDebugMessage(&players[0]);
        printDebugMessage(&players[1]);
        if (playersPlaying >= 3) printDebugMessage(&players[2]);
        if (playersPlaying >= 4) printDebugMessage(&players[3]);
        printf("\n");
    }

    // switch boolet type on all players to the next one on button press
    if (IsKeyPressed(KEY_F2)) {
        for (int i = 0; i < playerCount; ++i) {
            players[i].booletType = (players[i].booletType + 1) % booletTypeCount;
        }
    }

    // read the code bro
    if (IsKeyPressed(KEY_F3)) showFPS = !showFPS;

    // change level to next one in list
    if (IsKeyPressed(KEY_F4)) currentLevelIndex = (currentLevelIndex + 1) % levelCount;

    // go to level editor
    if (IsKeyPressed(KEY_F5)) gotoLevelEditor = true;
}

// this function handles drawing of all elements on the window
void DrawGameplayScreen(void) {
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), ColorFromHSV(180, 0.1, 0.1));
    DrawRectanglePro(bgRect1, center, 0.7 * sinf(GetTime()), ColorFromHSV(180, 0.1, 0.12));
    DrawRectanglePro(bgRect2, center, 0.7 * sinf(GetTime() + PI / 3), ColorFromHSV(180, 0.1, 0.14));
    if (playersCurrentlyPlaying < 2) {
        int fontSize = 128;
        int textWidth = MeasureTextEx(GetFontDefault(), "WAITING FOR PLAYERS", fontSize, 10).x;
        DrawTextPro(GetFontDefault(), "WAITING FOR PLAYERS",
                    (Vector2) {screenWidth / 2, screenHeight / 2},
                    (Vector2) {textWidth / 2, fontSize / 2}, 0.7 * sinf(GetTime() + 2 * PI / 3), fontSize, 10,
                    ColorFromHSV(180, 0.1, 0.2));
    }
    for (int i = 0; i < 4; ++i) if (players[i].isPlaying && !players[i].isDead) DrawPlayerTail(&players[i]);
    for (int i = 0; i < maxBooletsOnMap; ++i) if (boolets[i].enabled) DrawBoolet(&boolets[i]);
    for (int i = 0; i < 4; ++i) {
        if (!players[i].isPlaying) continue;
        DrawPlayerScore(&players[i]);
        if (!players[i].isDead) DrawPlayer(&players[i]);
    }
    for (int i = 0; i < maxWallCount; ++i)
        if (levels[currentLevelIndex].walls[i].enabled)
            DrawWall(&levels[currentLevelIndex].walls[i]);
    if (gameState == CHOOSEDOOR) for (int i = 0; i < 4; ++i) DrawDoor(&doors[i]);

    if (showFPS) {
        sprintf(fpsString, "%d", GetFPS());
        DrawText(fpsString, 2, 2, 14, GREEN);
    }
}

bool GotoLevelEditorScreen(void) {
    return gotoLevelEditor;
}
