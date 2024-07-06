#include "raylib.h"
#include "screens.h"
#include "constants.h"

#include "entities/player.h"
#include "entities/boolet.h"
#include <stdio.h>

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static char finishScreen = 0;
struct Player players[playerCount];
struct Boolet boolets[maxBooletsOnMap];
static int nextBooletIndex = 0;
static GameState gameState = FIGHT;

//----------------------------------------------------------------------------------
// Gameplay Screen Functions Definition
//----------------------------------------------------------------------------------

// Gameplay Screen Initialization logic
void InitGameplayScreen(void) {
    struct Player player1, player2, player3, player4;
    InitPlayerDefaults(&player1, TOPLEFT, KEY_W, KEY_S, KEY_A, KEY_D, KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT);
    players[0] = player1;
    InitPlayerDefaults(&player2, TOPRIGHT, KEY_I, KEY_K, KEY_J, KEY_L, KEY_I, KEY_K, KEY_J, KEY_L);
    players[1] = player2;
    InitPlayerDefaults(&player3, BOTTOMLEFT, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL);
    players[2] = player3;
    InitPlayerDefaults(&player4, BOTTOMRIGHT, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL);
    players[3] = player4;

    // TODO: for testing purposes only - kill player 3 and 4 instantly
    players[2].isDead = true;
    players[3].isDead = true;

    for (int i = 0; i < maxBooletsOnMap; ++i) boolets[i].enabled = false;

    finishScreen = 0;
}

bool isOutOfWindowBounds(Rectangle r) {
    return (r.x < -r.width || r.x > screenWidth || r.y < r.height || r.y > screenHeight);
}

void updateGameplayScreenDuringFight() {
    for (int i = 0; i < 4; ++i) {
        if (players[i].isDead) continue;
        ProcessPlayerInput(&players[i]);
        ApplyPlayerVelocity(&players[i]);
        if (IsPlayerShooting(&players[i])) {
            InitBooletDefaults(
                    &boolets[nextBooletIndex++], &players[i],
                    players[i].rect.x + players[i].rect.width / 2, players[i].rect.y + players[i].rect.height / 2,
                    5, players[i].shootingDirection.x, players[i].shootingDirection.y, 1, players[i].bulletSpeed, STRAIGHT
            );
            nextBooletIndex %= maxBooletsOnMap;
        }
    }

    for (int i = 0; i < maxBooletsOnMap; ++i) {
        ApplyBooletVelocity(&boolets[i]);
        if (isOutOfWindowBounds(boolets[i].rect)) boolets[i].enabled = false;
        if (!boolets[i].enabled) continue;
        for (int j = 0; j < 4; ++j) {
            if (!players[j].isDead && &players[j] != boolets[i].parent && CheckCollisionRecs(boolets[i].rect, players[j].rect)) {
                printf("hi");
                ApplyDamageToPlayer(&players[j], boolets[i].damage);
                boolets[i].enabled = false;
            }
        }
    }
}

void updateGameplayScreenDuringChooseDoor() {
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
}

// Gameplay Screen Draw logic
void DrawGameplayScreen(void) {
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), ColorFromHSV((int)(30 * GetTime()) % 360, 1, .2));

    for (int i = 0; i < maxBooletsOnMap; ++i) {
        if (boolets[i].enabled) DrawBoolet(&boolets[i]);
    }

    for (int i = 0; i < 4; ++i) {
        if (!players[i].isDead) DrawPlayer(&players[i]);
    }
}

// Gameplay Screen Unload logic
void UnloadGameplayScreen(void) {
    // Unload GAMEPLAY screen variables here!
}

// Gameplay Screen - should finish?
int FinishGameplayScreen(void) {
    return finishScreen;
}
