#include "raylib.h"
#include "raymath.h"
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

Texture2D texCrown;
bool playGameCrownAnim = false;
int playerGameWinnerIndex = -1;
int playerRoundWinnerIndex = -1;
float sfxWinPlayStartTime = -1000;
char winString[64];

Camera2D camera = {0};
static Location transitionStartLoc;
static RenderTexture2D transitionOldScreen;
float transitionStartTime = 0;
float transitionTime = 1;

static GameState gameState = FIGHT;
static Rectangle bgRect1;
static Rectangle bgRect2;
static Vector2 center;

struct Player players[playerCount];
struct Boolet boolets[maxBooletsOnMap];
struct Door doors[4];

float hueRotationSpeed = 40;
float hueRotationTimer = 0;

int currentLevelIndex = -1;
static int nextBooletIndex = 0;
static char playersCurrentlyPlaying;

bool gotoLevelEditor = false;
bool gotoMainMenu = false;

// functions definition below ------------------------------------------------------------------------------------------

float calculateTransitionTime(float t) {
    float sqr = t * t;
    return sqr / (2.0f * (sqr - t) + 1.0f);
}

// this function initializes the gameplay screen
void InitGameplayScreen(void) {
    texCrown = LoadTexture("resources/sprites/crown.png");
    playerGameWinnerIndex = -1;
    playerRoundWinnerIndex = -1;

    transitionOldScreen = LoadRenderTexture(screenWidth, screenHeight);
    camera.target = (Vector2) {screenWidth / 2, screenHeight / 2};
    camera.offset = (Vector2) {screenWidth / 2, screenHeight / 2};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    HideCursor();
    gotoLevelEditor = false;

    gotoMainMenu = false;

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

    for (int i = 0; i < playerCount; ++i) {
        players[i].isPlaying = false;
    }
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

    switch (inputState) {
        case GAMEPAD_ONLY:
            playersPlaying = 0;
            for (int i = 0; i < playerCount; ++i) players[i].isPlaying = false;
            break;
        case KEYBOARD_ONLY:
            playersPlaying = 2;
            players[2].isPlaying = false;
            players[3].isPlaying = false;
            break;
        case MIXED:
            playersPlaying = 1;
            for (int i = 1; i < playerCount; ++i) players[i].isPlaying = false;
            players[0].keyShootUp = KEY_UP;
            players[0].keyShootDown = KEY_DOWN;
            players[0].keyShootLeft = KEY_LEFT;
            players[0].keyShootRight = KEY_RIGHT;
            players[1].keyShootUp = -1;
            players[1].keyShootDown = -1;
            players[1].keyShootLeft = -1;
            players[1].keyShootRight = -1;
            players[1].keyMoveUp = -1;
            players[1].keyMoveDown = -1;
            players[1].keyMoveLeft = -1;
            players[1].keyMoveRight = -1;
            break;
    }

    for (int i = 0; i < maxBooletsOnMap; ++i) boolets[i].enabled = false;

    doors[0].location = LEFT;
    doors[1].location = RIGHT;
    doors[2].location = TOP;
    doors[3].location = BOTTOM;
    for (int i = 0; i < 4; ++i) InitDoorsWithRandomEffect(&doors[i]);

    shuffleLevelArray(levels, levelCount);
    currentLevelIndex = -1;
    resetLevel();
    currentLevelIndex = -1;
}

// this function resets the players and map for a new round of the game
void resetLevel() {
    if (currentLevelIndex >= levelCount - 1) {
        shuffleLevelArray(levels, levelCount);
    }
    char atCornerPhase = rand() % 4;
    Location possibleSpawnLocations[] = {
            TOPLEFT, TOPRIGHT,
            BOTTOMLEFT, BOTTOMRIGHT
    };

    BooletType bType = (BooletType) (rand() % booletTypeCount);
    for (int i = 0; i < playerCount; ++i) {
        if (players[i].wins >= winsNeededToWinGame) {
            players[i].totalGameWins++;
            sprintf(winString, "THE DUDE WITH THIS COLOR WINS (this game) :)");
            playerGameWinnerIndex = i;
            playGameCrownAnim = true;
            playerRoundWinnerIndex = -1;
            for (int j = 0; j < playerCount; ++j) {
                ClearPlayerOfEffects(&players[j]);
                SetWinToZeroPlayer(&players[j]);
            }
        }
        SetPlayerLocation(&players[i], possibleSpawnLocations[(i + atCornerPhase) % 4]);
        players[i].isDead = false;
        players[i].velocity.x = 0;
        players[i].velocity.y = 0;
        players[i].lastDodgeTime = GetTime();
        players[i].lastShotTime = GetTime();
        players[i].health = players[i].maxHealth;

        if (randomizeEffectsEveryRound) {
            for (int j = 0; j < playerEffectCapacityAndLifespan; ++j) {
                AssignEffectToPlayer(rand() % playerEffectCount, &players[i]);
            }
        }

        players[i].booletType = bType;
        if (!playersUseTheSameWeapon) {
            bType = (BooletType) (rand() % booletTypeCount);
        }

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

    int borderWallThickness = 3;
    levels[currentLevelIndex].walls[maxWallCount - 4] = (struct Wall) {
            (Rectangle) {0, 0, borderWallThickness, screenHeight}, true
    };
    levels[currentLevelIndex].walls[maxWallCount - 3] = (struct Wall) {
            (Rectangle) {0, 0, screenWidth, borderWallThickness}, true
    };
    levels[currentLevelIndex].walls[maxWallCount - 2] = (struct Wall) {
            (Rectangle) {screenWidth - borderWallThickness, 0, borderWallThickness, screenHeight}, true
    };
    levels[currentLevelIndex].walls[maxWallCount - 1] = (struct Wall) {
            (Rectangle) {0, screenHeight - borderWallThickness, screenWidth, borderWallThickness}, true
    };
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

// this function updates the game regardless of current gameState
void UpdateGameplayScreen(void) {

    // handle hue rotation and animation stuff -------------------------------------------------------------------------
    hueRotationTimer += GetFrameTime() * hueRotationSpeed;
    if (hueRotationSpeed > defaultHueRotationSpeed) {
        hueRotationSpeed = defaultHueRotationSpeed + 0.96 * (hueRotationSpeed - defaultHueRotationSpeed);
    }

    if (GetTime() - sfxWinPlayStartTime < 5.1 &&
        IsMusicStreamPlaying(currentMusicIndex >= 0 ? bgMusic[currentMusicIndex] : startMusic)) {
        PauseMusicStream(currentMusicIndex >= 0 ? bgMusic[currentMusicIndex] : startMusic);
    } else if (GetTime() - sfxWinPlayStartTime > 5.1 &&
               !IsMusicStreamPlaying(currentMusicIndex >= 0 ? bgMusic[currentMusicIndex] : startMusic)) {
        ResumeMusicStream(currentMusicIndex >= 0 ? bgMusic[currentMusicIndex] : startMusic);
    }

    // new round transition --------------------------------------------------------------------------------------------

    if (gameState == TRANSITION) {
        float t = 1.5 * (GetTime() - transitionStartTime);
        transitionTime = calculateTransitionTime(t);

        if (t >= 1) {
            gameState = FIGHT;
            camera.offset = (Vector2) {screenWidth / 2, screenHeight / 2};
        }
    }

    // look for gamepads -----------------------------------------------------------------------------------------------
    if (inputState != KEYBOARD_ONLY && playersPlaying < playerCount) {
        int startId = inputState == MIXED ? 1 : 0;
        for (int gamepadId = startId; gamepadId < playerCount; ++gamepadId) {
            if (!IsGamepadAvailable(gamepadId - startId)) {
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
        struct Player *p = &players[i];
        if (!playGameCrownAnim)
            ProcessPlayerInput(p, i + (inputState == MIXED ? -1 : 0) -
                                  (inputState == KEYBOARD_ONLY ? playerCount : 0));
        ApplyPlayerVelocity(&players[i]);
        if ((currentLevelIndex >= 0 || i >= maxWallCount - 4)) {
            for (int j = 0; j < maxWallCount; ++j) {
                if (CheckCollisionRecs(levels[currentLevelIndex].walls[j].rect, players[i].rect)) {
                    FixPlayerPositionAgainstWall(&levels[currentLevelIndex].walls[j], &players[i]);
                }
            }
        }
        if (!players[i].isDead) playersAlive++;
        else continue;
        if (gameState == CHOOSEDOOR && !players[i].isDead) {
            if (GetTime() - doors[0].timeSpawned > 1)
                for (int j = 0; j < 4; ++j) {
                    if (CheckCollisionRecs(doors[j].finalRect, players[i].rect)) {
                        if (doors[j].playerEffect == RANDOM_EFFECT_TO_EVERYONE)
                            for (int k = 0; k < playerCount; ++k)
                                AssignEffectToPlayer(rand() % playerEffectCount, &players[k]);
                        else if (doors[j].isDebuff) AssignEffectToPlayer(doors[j].playerEffect, &players[i]);
                        else {
                            int r = i;
                            while (r == i) r = rand() % playersPlaying;
                            AssignEffectToPlayer(doors[j].playerEffect, &players[r]);
                        }
                        for (int k = 0; k < maxBooletsOnMap; ++k) boolets[k].enabled = false;


                        BeginTextureMode(transitionOldScreen);
                        ClearBackground(RAYWHITE);
                        DrawGameplayScreen(true);
                        EndTextureMode();

                        transitionStartLoc = doors[j].location;
                        transitionStartTime = GetTime();
                        transitionTime = 0;
                        gameState = TRANSITION;
                        int px = players[i].rect.x;
                        int py = players[i].rect.y;
                        resetLevel();
                        if (players[i].wins > 0)
                            switch (transitionStartLoc) {
                                case LEFT:
                                    players[i].rect.x = px + screenWidth - players[i].rect.width;
                                    players[i].rect.y = py;
                                    break;
                                case RIGHT:
                                    players[i].rect.x = px - screenWidth + players[i].rect.width;
                                    players[i].rect.y = py;
                                    break;
                                case TOP:
                                    players[i].rect.x = px;
                                    players[i].rect.y = py + screenHeight - players[i].rect.height;
                                    break;
                                case BOTTOM:
                                    players[i].rect.x = px;
                                    players[i].rect.y = py - screenHeight + players[i].rect.height;
                                    break;
                            }
                        playerRoundWinnerIndex = i;
                        return;
                    }
                }
        }

        if (IsPlayerShooting(p) && !playGameCrownAnim) {
            BooletType btype = p->booletType;
            if (btype == RANDOM) {
                do btype = rand() % booletTypeCount;
                while (btype == RANDOM);
            }
            if (btype == HITSCAN) {
                int x = p->rect.x + p->rect.width / 2;
                int y = p->rect.y + p->rect.height / 2;
                Rectangle r;
                int delta = 15 + p->booletSize;
                do {
                    x += p->shootingDirection.x * delta;
                    y += p->shootingDirection.y * delta;
                    r = (Rectangle) {x, y, p->booletSize, p->booletSize};
                    for (int j = 0; j < maxWallCount; ++j)
                        if (currentLevelIndex >= 0 &&
                            CheckCollisionRecs(r, levels[currentLevelIndex].walls[j].rect))
                            r.x = -1000;
                    for (int j = 0; j < playerCount; ++j)
                        if (&players[j] != p && !players[j].isDead && players[j].isPlaying &&
                            CheckCollisionRecs(r, players[j].rect))
                            r.x = -1000;
                    InitBooletDefaults(
                            &boolets[nextBooletIndex++], p, x, y,
                            p->booletSize >= 2 ? p->booletSize : 2, p->shootingDirection.x, p->shootingDirection.y,
                            1, 0, btype, ColorFromHSV(p->huePhase, .8, 1), p->booletAmplitude, 0.3
                    );
                    nextBooletIndex %= maxBooletsOnMap;
                } while (isOutOfWindowBounds(r) == -1);
            } else if (btype == SHOTGUN) {
                for (float q = -3.0f / 2.0f; q < 2; ++q) {
                    Vector2 outDir = (Vector2) {
                            p->shootingDirection.x,
                            p->shootingDirection.y
                    };
                    outDir = Vector2Rotate(outDir, q / (p->booletAmplitude > 2 ? p->booletAmplitude : 2) / 3 * 2);
                    InitBooletDefaults(
                            &boolets[nextBooletIndex++], p,
                            p->rect.x + p->rect.width / 2, p->rect.y + p->rect.height / 2,
                            p->booletSize >= 2 ? p->booletSize : 2, outDir.x, outDir.y, 1, p->bulletSpeed,
                            btype, ColorFromHSV(p->huePhase, .8, 1), p->booletAmplitude, p->booletDecayTimeLeft
                    );
                    nextBooletIndex %= maxBooletsOnMap;
                    p->sfxShootSoundIndex %= sfxShootCount;
                }
                PlaySound(sfxShoot[p->sfxShootSoundIndex++]);
            } else if (btype == INVERTED) {
                int x = p->rect.x + p->rect.width / 2;
                int y = p->rect.y + p->rect.height / 2;
                int delta = p->booletSize;
                Rectangle r = (Rectangle) {x, y, p->booletSize, p->booletSize};
                Rectangle rNext = (Rectangle) {x, y, p->booletSize, p->booletSize};
                rNext.x += p->shootingDirection.x * delta;
                rNext.y += p->shootingDirection.y * delta;
                while (isOutOfWindowBounds(rNext) == -1) {
                    bool isCollidingWall = false;
                    for (int j = 0; j < maxWallCount; ++j) {
                        if (currentLevelIndex >= 0 &&
                            CheckCollisionRecs(rNext, levels[currentLevelIndex].walls[j].rect)) {
                            isCollidingWall = true;
                            break;
                        }
                    }
                    if (isCollidingWall) break;
                    r = rNext;
                    rNext.x += p->shootingDirection.x * delta;
                    rNext.y += p->shootingDirection.y * delta;
                }
                InitBooletDefaults(
                        &boolets[nextBooletIndex++], p,
                        r.x, r.y,
                        p->booletSize >= 2 ? p->booletSize : 2, p->shootingDirection.x,
                        p->shootingDirection.y, 1, p->bulletSpeed,
                        btype, ColorFromHSV(p->huePhase, .8, 1), p->booletAmplitude, p->booletDecayTimeLeft
                );
                nextBooletIndex %= maxBooletsOnMap;
                PlaySound(sfxShoot[p->sfxShootSoundIndex++]);
                p->sfxShootSoundIndex %= sfxShootCount;
            } else {
                InitBooletDefaults(
                        &boolets[nextBooletIndex++], p,
                        p->rect.x + p->rect.width / 2, p->rect.y + p->rect.height / 2,
                        p->booletSize >= 2 ? p->booletSize : 2, p->shootingDirection.x,
                        p->shootingDirection.y, 1, p->bulletSpeed,
                        btype, ColorFromHSV(p->huePhase, .8, 1), p->booletAmplitude, p->booletDecayTimeLeft
                );
                nextBooletIndex %= maxBooletsOnMap;
                PlaySound(sfxShoot[p->sfxShootSoundIndex++]);
                p->sfxShootSoundIndex %= sfxShootCount;
            }
        }
    }

    // if there is 1 or 0 players alive --------------------------------------------------------------------------------
    if (playersCurrentlyPlaying >= 2 && playersAlive == 0) {
        gameState = FIGHT;
        resetLevel();
    } else if (gameState == FIGHT && playersAlive == 1 && playersCurrentlyPlaying >= 2) {
        struct Player *alivePlayer;
        for (int k = 0; k < 4; ++k) {
            if (!players[k].isDead && players[k].isPlaying) {
                alivePlayer = &players[k];
                break;
            }
        }
        if (playersAlive == 1) {
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
                }
            } else if (boolets[i].type == EXPLODING) {
                MoveBulletBackOneStep(&boolets[i]);
                ExplodeBoolet(&boolets[i], &nextBooletIndex, boolets, STRAIGHT);
            } else boolets[i].enabled = false;
        }
        ApplyBooletVelocity(&boolets[i]);
        for (int j = 0; j < maxWallCount; ++j)
            if ((currentLevelIndex >= 0 || i >= maxWallCount - 4) &&
                CheckWallBooletCollisionAndFixPosition(&levels[currentLevelIndex].walls[j], &boolets[i])) {
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

    // go to main menu
    if (IsKeyPressed(KEY_ESCAPE)) {
        gotoMainMenu = true;
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

    // read the code
    if (IsKeyPressed(KEY_F6)) currentMusicIndex = (currentMusicIndex + 1) % bgMusicCount;

    // add crowns
    if (IsKeyPressed((KEY_F7))) for (int i = 0; i < playerCount; ++i) players[i].totalGameWins++;
}

// this function handles drawing of all elements on the window
void DrawGameplayScreen(bool overrideMode) {
    if (gameState == TRANSITION) {
        switch (transitionStartLoc) {
            case LEFT:
                camera.offset.x = -screenWidth * (1 - transitionTime) + screenWidth / 2;
                break;
            case RIGHT:
                camera.offset.x = screenWidth * (1 - transitionTime) + screenWidth / 2;
                break;
            case TOP:
                camera.offset.y = -screenHeight * (1 - transitionTime) + screenHeight / 2;
                break;
            case BOTTOM:
                camera.offset.y = screenHeight * (1 - transitionTime) + screenHeight / 2;
                break;
        }
    }

    if (!overrideMode) BeginMode2D(camera);

    DrawTexture(texBackground, 0, 0, WHITE);

    if (gameState == TRANSITION) {
        switch (transitionStartLoc) {
            case LEFT:
                DrawTexturePro(
                        transitionOldScreen.texture,
                        (Rectangle) {0, 0, transitionOldScreen.texture.width, -transitionOldScreen.texture.height},
                        (Rectangle) {screenWidth, 0, screenWidth, screenHeight},
                        (Vector2) {0, 0},
                        0, WHITE
                );
                break;
            case RIGHT:
                DrawTexturePro(
                        transitionOldScreen.texture,
                        (Rectangle) {0, 0, transitionOldScreen.texture.width, -transitionOldScreen.texture.height},
                        (Rectangle) {-screenWidth, 0, screenWidth, screenHeight},
                        (Vector2) {0, 0},
                        0, WHITE
                );
                break;
            case TOP:
                DrawTexturePro(
                        transitionOldScreen.texture,
                        (Rectangle) {0, 0, transitionOldScreen.texture.width, -transitionOldScreen.texture.height},
                        (Rectangle) {0, screenHeight, screenWidth, screenHeight},
                        (Vector2) {0, 0},
                        0, WHITE
                );
                break;
            case BOTTOM:
                DrawTexturePro(
                        transitionOldScreen.texture,
                        (Rectangle) {0, 0, transitionOldScreen.texture.width, -transitionOldScreen.texture.height},
                        (Rectangle) {0, -screenHeight, screenWidth, screenHeight},
                        (Vector2) {0, 0},
                        0, WHITE
                );
                break;
        }
    }

    playersCurrentlyPlaying = 0;
    for (int i = 0; i < playerCount; ++i) {
        if (players[i].isPlaying) {
            playersCurrentlyPlaying++;
            DrawPlayerTail(&players[i]);
        }
        if (!players[i].isPlaying || !players[i].isDead) continue;
        DrawPlayer(&players[i]);
    }
    for (int i = 0; i < maxBooletsOnMap; ++i) if (boolets[i].enabled) DrawBoolet(&boolets[i]);
    for (int i = 0; i < maxWallCount; ++i) {
        if ((currentLevelIndex >= 0 || i >= maxWallCount - 4) && levels[currentLevelIndex].walls[i].enabled) {
            DrawWall(&levels[currentLevelIndex].walls[i]);
        }
    }

    for (int i = 0; i < playerCount; ++i) {
        if (!players[i].isPlaying || players[i].isDead) continue;
        DrawPlayerScore(&players[i]);
        DrawPlayer(&players[i]);
    }
    for (int i = 0; i < playerCount; ++i) {
        if (players[i].isDead || !players[i].isPlaying) continue;
        if (playerRoundWinnerIndex == i) {
            Vector2 crownPos = {
                    players[i].pastPos[2].x + texCrown.width / 8,
                    players[i].pastPos[2].y - texCrown.height / 8 -
                    screenHeight * (1 - (transitionTime <= 1 ? transitionTime : 1))
            };
            int rotMod = 0;
            if (players[i].totalGameWins > 0) {
                crownPos.x += texCrown.width / 12;
                rotMod = 15;
            }
            DrawTexturePro(
                    texCrown,
                    (Rectangle) {0, 0, texCrown.width, texCrown.height},
                    (Rectangle) {crownPos.x, crownPos.y, texCrown.width / 8, texCrown.height / 8},
                    (Vector2) {texCrown.width / 16, texCrown.height / 16}, 6 * sin(6 * GetTime()) + rotMod, WHITE
            );
        }
        for (int j = 0; j < players[i].totalGameWins; ++j) {

            float rotation;
            Vector2 crownPos;
            Rectangle dest = {-1, -1, 1, 1};

            if (playGameCrownAnim && i == playerGameWinnerIndex && j == players[i].totalGameWins - 1) {
                if (!overrideMode) EndMode2D();

                rotation = 0;
                crownPos.x = -10;
                crownPos.y = -10;

                float t = (GetTime() - transitionStartTime) - 0.8;
                char tStr[16];
                DrawText(tStr, 10, 10, 32, WHITE);
                float animationDuration = 5;

                if (t < 0) continue;
                else if (t < 0.2 * animationDuration) {
                    if (!IsSoundPlaying(sfxWin)) {
                        PlaySound(sfxWin);
                        sfxWinPlayStartTime = GetTime();
                    }

                    float tranT = calculateTransitionTime(
                            (0.2 * animationDuration - t)
                    );
                    dest = (Rectangle) {
                            (screenWidth - texCrown.width) / 2,
                            (screenHeight - 2 * texCrown.height) / 2 + screenHeight * tranT,
                            texCrown.width, texCrown.height
                    };
                    rotation = 0;
                    DrawRectangle(0, 0, screenWidth, screenHeight, ColorAlpha(BLACK, 0.85 * (1 - tranT)));
                    int tW = MeasureText(winString, 64);
                    DrawText(winString, (screenWidth - tW) / 2, screenHeight / 2 + screenHeight * tranT, 64,
                             ColorFromHSV(players[i].huePhase, 1, 1));
                } else if (t < 0.6 * animationDuration) {
                    dest = (Rectangle) {
                            (screenWidth - texCrown.width) / 2,
                            (screenHeight - 2 * texCrown.height) / 2,
                            texCrown.width, texCrown.height
                    };
                    DrawRectangle(0, 0, screenWidth, screenHeight, ColorAlpha(BLACK, 0.85));
                    int tW = MeasureText(winString, 64);
                    DrawText(winString, (screenWidth - tW) / 2, screenHeight / 2, 64,
                             ColorFromHSV(players[i].huePhase, 1, 1));
                } else if (t < 0.8 * animationDuration) {
                    float tranT = calculateTransitionTime(
                            0.8 * animationDuration - t
                    );
                    dest = (Rectangle) {
                            (screenWidth - texCrown.width) / 2,
                            (screenHeight - 2 * texCrown.height) / 2 - screenHeight * (1 - tranT),
                            texCrown.width, texCrown.height
                    };
                    rotation = 0;
                    DrawRectangle(0, 0, screenWidth, screenHeight, ColorAlpha(BLACK, 0.85 * tranT));
                    int tW = MeasureText(winString, 64);
                    DrawText(winString, (screenWidth - tW) / 2, screenHeight / 2 - screenHeight * (1 - tranT),
                             64,
                             ColorFromHSV(players[i].huePhase, 1, 1));
                } else playGameCrownAnim = false;

                if (!overrideMode) BeginMode2D(camera);
            } // playCrownAnimation code
            else {
                float t = (GetTime() - transitionStartTime) - 0.1 * j - 4.8;
                if (players[playerRoundWinnerIndex].wins != 0) t = 1;
                float tranT = 1;
                if (t >= 0 && t < 1) tranT = calculateTransitionTime(t);
                else if (t < 0) tranT = 0;


                crownPos = (Vector2) {
                        players[i].rect.x + 20 - texCrown.width / 8,
                        players[i].rect.y + 28 - j * texCrown.height / 14 -
                        screenHeight * (1 - (tranT <= 1 ? tranT : 1))
                };
                dest = (Rectangle) {crownPos.x, crownPos.y, texCrown.width / 8, texCrown.height / 8};
                rotation = -35 - sinf(GetTime() + players[i].huePhase) * j;
            }

            if (dest.x != -1 && dest.y != -1)
                DrawTexturePro(
                        texCrown,
                        (Rectangle) {0, 0, texCrown.width, texCrown.height},
                        dest,
                        (Vector2) {-texCrown.width / 16, 25 + texCrown.height / 32 * j},
                        rotation,
                        WHITE
                );
        }
    }
    if (gameState == CHOOSEDOOR) for (int i = 0; i < 4; ++i) DrawDoor(&doors[i]);

    if (playersCurrentlyPlaying < 2 && gameState == FIGHT) {
        int fontSize = 140;
        int textWidth = MeasureTextEx(GetFontDefault(), "WAITING FOR PLAYERS", fontSize, 10).x;
        DrawTextPro(GetFontDefault(), "WAITING FOR PLAYERS",
                    (Vector2) {screenWidth / 2, screenHeight / 2},
                    (Vector2) {textWidth / 2, fontSize / 2}, 2 * sinf(GetTime() + 2 * PI / 3), fontSize, 10,
                    ColorAlpha(WHITE, 0.7));
    } else if (currentLevelIndex < 0 && gameState == FIGHT) {
        int fontSize = 140;
        int textWidth = MeasureTextEx(GetFontDefault(), "KILL YOUR FRIENDS", fontSize, 10).x;
        DrawTextPro(GetFontDefault(), "KILL YOUR FRIENDS",
                    (Vector2) {screenWidth / 2, screenHeight / 2},
                    (Vector2) {textWidth / 2, fontSize / 2}, 2 * sinf(GetTime() + 2 * PI / 3), fontSize, 10,
                    ColorAlpha(WHITE, 0.7));
    } else if (currentLevelIndex < 0 && gameState == CHOOSEDOOR) {
        int fontSize = 140;
        int textWidth = MeasureTextEx(GetFontDefault(), "CHOOSE A DOOR", fontSize, 10).x;
        DrawTextPro(GetFontDefault(), "CHOOSE A DOOR",
                    (Vector2) {screenWidth / 2, screenHeight / 2},
                    (Vector2) {textWidth / 2, fontSize / 2}, 2 * sinf(GetTime() + 2 * PI / 3), fontSize, 10,
                    ColorAlpha(WHITE, 0.7));
    }

    if (!overrideMode) EndMode2D();

    if (showFPS) {
        DrawFPS(10, 10);
    }
}

void UnloadGameplayScreen(void) {
    UnloadTexture(texCrown);
}

bool GotoLevelEditorScreen(void) {
    return gotoLevelEditor;
}

bool GotoMainMenuScreen(void) {
    return gotoMainMenu;
}
