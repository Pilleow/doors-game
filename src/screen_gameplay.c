#include "raylib.h"
#include "screens.h"
#include <stdlib.h>
#include <stdio.h>

#include "constants.h"
#include "entities/door.h"
#include "entities/player.h"
#include "entities/boolet.h"
#include "entities/playerEffect.h"

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
struct Player players[playerCount];
struct Boolet boolets[maxBooletsOnMap];
struct Door doors[4];
static int nextBooletIndex = 0;
static GameState gameState = FIGHT;
static char fpsString[16];

//----------------------------------------------------------------------------------
// Gameplay Screen Functions Definition
//----------------------------------------------------------------------------------

// Gameplay Screen Initialization logic
void InitGameplayScreen(void) {
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

    if (playersPlaying < 3) players[2].isPlaying = false;
    if (playersPlaying < 4) players[3].isPlaying = false;

    for (int i = 0; i < maxBooletsOnMap; ++i) boolets[i].enabled = false;

    doors[0].location = LEFT;
    doors[1].location = RIGHT;
    doors[2].location = TOP;
    doors[3].location = BOTTOM;
    for (int i = 0; i < 4; ++i) {
        InitDoorsWithRandomEffect(&doors[i]);
    }
}

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
    }
    if (playersPlaying < 3) players[2].isPlaying = false;
    if (playersPlaying < 4) players[3].isPlaying = false;
    for (int i = 0; i < 4; ++i) {
        InitDoorsWithRandomEffect(&doors[i]);
    }
}

bool isOutOfWindowBounds(Rectangle r) {
    return (r.x<-r.width || r.x>(
    float)screenWidth || r.y<r.height || r.y>(
    float)screenHeight);
}

void updateGameplayScreenDuringFight() {
    for (int i = 0; i < 4; ++i) {
        if (!players[i].isPlaying || players[i].isDead) continue;
        ProcessPlayerInput(&players[i]);
        ApplyPlayerVelocity(&players[i]);
        if (IsPlayerShooting(&players[i])) {
            InitBooletDefaults(
                    &boolets[nextBooletIndex++], &players[i],
                    players[i].rect.x + players[i].rect.width / 2, players[i].rect.y + players[i].rect.height / 2,
                    5, players[i].shootingDirection.x, players[i].shootingDirection.y, 1, players[i].bulletSpeed,
                    STRAIGHT, ColorFromHSV(players[i].huePhase, 1, 1)
            );
            nextBooletIndex %= maxBooletsOnMap;
        }
    }

    for (int i = 0; i < maxBooletsOnMap; ++i) {
        if (!boolets[i].enabled) continue;
        if (isOutOfWindowBounds(boolets[i].rect)) boolets[i].enabled = false;
        ApplyBooletVelocity(&boolets[i]);
        for (int j = 0; j < 4; ++j) {
            if (!players[j].isDead && players[j].isPlaying && &players[j] != boolets[i].parent &&
                CheckCollisionRecs(boolets[i].rect, players[j].rect)) {
                ApplyDamageToPlayer(&players[j], boolets[i].damage);
                boolets[i].enabled = false;
                char playersAlive = 0;
                struct Player *alivePlayer;
                for (int k = 0; k < 4; ++k) {
                    if (!players[k].isDead && players[k].isPlaying) {
                        alivePlayer = &players[k];
                        playersAlive++;
                    }
                }
                if (playersAlive == 0) resetLevel();
                else if (playersAlive == 1) {
                    AddWinToPlayer(alivePlayer);
                    gameState = CHOOSEDOOR;
                    for (int k = 0; k < 4; ++k) doors[k].timeSpawned = GetTime();
                }
            }
        }
    }
}

void updateGameplayScreenDuringChooseDoor() {
    for (int i = 0; i < 4; ++i) {
        if (players[i].isDead || !players[i].isPlaying) continue;
        ProcessPlayerInput(&players[i]);
        ApplyPlayerVelocity(&players[i]);

        for (int j = 0; j < 4; ++j) {
            if (CheckCollisionRecs(doors[j].finalRect, players[i].rect)) {
                if (doors[j].playerEffect == CLEAR_ALL_EFFECTS)
                    for (int k = 0; k < playerCount; ++k) ClearPlayerOfEffects(&players[k]);
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

    for (int i = 0; i < maxBooletsOnMap; ++i) {
        if (!boolets[i].enabled) continue;
        if (isOutOfWindowBounds(boolets[i].rect)) boolets[i].enabled = false;
        ApplyBooletVelocity(&boolets[i]);
    }
}

// Gameplay Screen Update logic
void UpdateGameplayScreen(void) {
    switch (gameState) {
        case FIGHT:
            updateGameplayScreenDuringFight();
            break;
        case CHOOSEDOOR:
            updateGameplayScreenDuringChooseDoor();
            break;
    }

    if (IsKeyPressed(KEY_F3)) {
        printDebugMessage(&players[0]);
        printDebugMessage(&players[1]);
        if (playersPlaying >= 3) printDebugMessage(&players[2]);;
        if (playersPlaying >= 4) printDebugMessage(&players[3]);;
        printf("\n");
    }
}

// Gameplay Screen Draw logic
void DrawGameplayScreen(void) {
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), ColorFromHSV(180, 0.1, 0.1));
    for (int i = 0; i < maxBooletsOnMap; ++i) if (boolets[i].enabled) DrawBoolet(&boolets[i]);
    if (gameState == CHOOSEDOOR) for (int i = 0; i < 4; ++i) DrawDoor(&doors[i]);
    for (int i = 0; i < 4; ++i) {
        if (!players[i].isPlaying) continue;
        DrawPlayerScore(&players[i]);
        if (!players[i].isDead) DrawPlayer(&players[i]);
    }

    if (showFPS) {
        sprintf(fpsString, "%d", GetFPS());
        DrawText(fpsString, 2, 2, 14, GREEN);
    }
}

// Gameplay Screen Unload logic
void UnloadGameplayScreen(void) {
    // Unload GAMEPLAY screen variables here!
}
