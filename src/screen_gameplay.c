#include "raylib.h"
#include "screens.h"
#include "constants.h"
#include <stdlib.h>

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
static int lastDebugInfoTime = 0;

//----------------------------------------------------------------------------------
// Gameplay Screen Functions Definition
//----------------------------------------------------------------------------------

// Gameplay Screen Initialization logic
void InitGameplayScreen(void) {
    struct Player player1, player2, player3, player4;
    InitPlayerDefaults(&player1,
                       TOPLEFT,
                       KEY_W, KEY_S, KEY_A, KEY_D,
                       KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT,
                       KEY_LEFT_SHIFT
    );
    players[0] = player1;
    InitPlayerDefaults(&player2,
                       TOPRIGHT,
                       KEY_I, KEY_K, KEY_J, KEY_L,
                       KEY_I, KEY_K, KEY_J, KEY_L,
                       KEY_SPACE
    );
    players[1] = player2;
    InitPlayerDefaults(&player3,
                       BOTTOMLEFT, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL,
                       KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL,
                       KEY_NULL
    );
    players[2] = player3;
    InitPlayerDefaults(&player4,
                       BOTTOMRIGHT,
                       KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL,
                       KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL,
                       KEY_NULL
    );
    players[3] = player4;

    if (playersPlaying < 4) players[2].isDead = true;
    if (playersPlaying < 3) players[1].isDead = true;

    for (int i = 0; i < maxBooletsOnMap; ++i) boolets[i].enabled = false;

    doors[0].location = LEFT;
    doors[1].location = RIGHT;
    doors[2].location = TOP;
    doors[3].location = BOTTOM;
    for (int i = 0; i < 4; ++i) {
        InitDoorsWithRandomEffect(&doors[i]);
    }
}

void resetPlayersButDoNotTouchEffects() {
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
    if (playersPlaying < 4) players[2].isDead = true;
    if (playersPlaying < 3) players[1].isDead = true;
}

bool isOutOfWindowBounds(Rectangle r) {
    return (r.x<-r.width || r.x>(
    float)screenWidth || r.y<r.height || r.y>(
    float)screenHeight);
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
                    5, players[i].shootingDirection.x, players[i].shootingDirection.y, 1, players[i].bulletSpeed,
                    STRAIGHT
            );
            nextBooletIndex %= maxBooletsOnMap;
        }
    }

    for (int i = 0; i < maxBooletsOnMap; ++i) {
        ApplyBooletVelocity(&boolets[i]);
        if (isOutOfWindowBounds(boolets[i].rect)) boolets[i].enabled = false;
        if (!boolets[i].enabled) continue;
        for (int j = 0; j < 4; ++j) {
            if (!players[j].isDead && &players[j] != boolets[i].parent &&
                CheckCollisionRecs(boolets[i].rect, players[j].rect)) {
                ApplyDamageToPlayer(&players[j], boolets[i].damage);
                boolets[i].enabled = false;
                char playersAlive = 0;
                for (int k = 0; k < 4; ++k) if (!players[k].isDead) playersAlive++;
                if (playersAlive <= 1) {
                    gameState = CHOOSEDOOR;
                    for (int k = 0; k < 4; ++k) doors[k].timeSpawned = GetTime();
                }
            }
        }
    }
}

void updateGameplayScreenDuringChooseDoor() {
    for (int i = 0; i < 4; ++i) {
        if (players[i].isDead) continue;
        ProcessPlayerInput(&players[i]);
        ApplyPlayerVelocity(&players[i]);

        for (int j = 0; j < 4; ++j) {
            if (CheckCollisionRecs(doors[j].finalRect, players[j].rect)) {
                resetPlayersButDoNotTouchEffects();
                if (
                        doors[j].playerEffect == EVERYONE_GETS_RANDOM_DEBUFFS
                        || doors[j].playerEffect == EVERYONE_GETS_RANDOM_BUFFS
                        || doors[j].playerEffect == EVERYONE_GETS_RANDOM_EFFECTS
                        )
                    for (int k = 0; k < playerCount; ++k) AssignEffectToPlayer(doors[j].playerEffect, &players[k]);
                else if (doors[j].playerEffect == CLEAR_ALL_EFFECTS)
                    for (int k = 0; k < playerCount; ++k) ClearPlayerOfEffects(&players[k]);
                else if (doors[j].isDebuff) AssignEffectToPlayer(doors[j].playerEffect, &players[j]);
                else if (doors[j].isDebuff) AssignEffectToPlayer(doors[j].playerEffect, &players[j]);

                for (int i = 0; i < maxBooletsOnMap; ++i) boolets[i].enabled = false;
                gameState = FIGHT;
            }
        }
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

    if (GetTime() - lastDebugInfoTime > 1) {
        for (int i = 0; i < player_speed_modifier; ++i) {

            if (playersPlaying >= 4) players[2];
            if (playersPlaying >= 3) players[1];
        }
        lastDebugInfoTime = GetTime();
    }
}

// Gameplay Screen Draw logic
void DrawGameplayScreen(void) {
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), ColorFromHSV(180, 0.1, 0.1));
    for (int i = 0; i < maxBooletsOnMap; ++i) if (boolets[i].enabled) DrawBoolet(&boolets[i]);
    for (int i = 0; i < 4; ++i) if (!players[i].isDead) DrawPlayer(&players[i]);
    if (gameState == CHOOSEDOOR) for (int i = 0; i < 4; ++i) DrawDoor(&doors[i]);
}

// Gameplay Screen Unload logic
void UnloadGameplayScreen(void) {
    // Unload GAMEPLAY screen variables here!
}
