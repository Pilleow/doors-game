#include "raylib.h"
#include "screens.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#include "constants.h"
#include "entities/wall.h"
#include "entities/door.h"
#include "entities/level.h"
#include "entities/player.h"
#include "entities/boolet.h"
#include "entities/playerEffect.h"

// local variable declaration below ------------------------------------------------------------------------------------

bool gotoGameplayScreenLE = false;
int snapSize = 1;
int levelIndex = levelCount;
int nextWallIndex = 0;
int clickCount = 0;
struct Wall walls[maxWallCount];
char nextWallIndexMinus1String[16];
extern struct Player players[playerCount];

// functions definition below ------------------------------------------------------------------------------------------

// this function initializes the gameplay screen
void InitLevelEditorScreen(void) {
    LoadAllLevels(levels, false);
    sprintf(nextWallIndexMinus1String, "%d / %d", nextWallIndex, maxWallCount);
    ShowCursor();
    clickCount = 0;
    nextWallIndex = 0;
    gotoGameplayScreenLE = false;
    snapSize = screenWidth / screenHeight * 30;
    for (int i = 0; i < maxWallCount; ++i) walls[i].enabled = false;

    struct Player player1, player2, player3, player4;
    InitPlayerDefaults(&player1,
                       TOPLEFT, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL,
                       KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL,
                       KEY_NULL
    );
    players[0] = player1;
    InitPlayerDefaults(&player2,
                       BOTTOMRIGHT, KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL,
                       KEY_NULL, KEY_NULL, KEY_NULL, KEY_NULL,
                       KEY_NULL
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
}

// this function updates the game regardless of current gameState
void UpdateLevelEditorScreen(void) {
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        Vector2 mousePos = GetMousePosition();
        mousePos.x = ((int) (mousePos.x / snapSize)) * snapSize;
        mousePos.y = ((int) (mousePos.y / snapSize)) * snapSize;
        if (clickCount % 2 == 0) {
            walls[nextWallIndex].enabled = false;
            walls[nextWallIndex].rect.x = mousePos.x;
            walls[nextWallIndex].rect.y = mousePos.y;
        } else {
            mousePos.x += snapSize;
            mousePos.y += snapSize;
            walls[nextWallIndex].rect.width = fabsf(mousePos.x - walls[nextWallIndex].rect.x);
            walls[nextWallIndex].rect.height = fabsf(mousePos.y - walls[nextWallIndex].rect.y);
            if (mousePos.x - walls[nextWallIndex].rect.x < 0) {
                walls[nextWallIndex].rect.x -= walls[nextWallIndex].rect.width;
                walls[nextWallIndex].rect.x -= snapSize;
                walls[nextWallIndex].rect.width += 2 * snapSize;
            }
            if (mousePos.y - walls[nextWallIndex].rect.y < 0) {
                walls[nextWallIndex].rect.y -= walls[nextWallIndex].rect.height;
                walls[nextWallIndex].rect.y -= snapSize;
                walls[nextWallIndex].rect.height += 2 * snapSize;
            }
            if (walls[nextWallIndex].rect.width != 0 && walls[nextWallIndex].rect.height != 0) {
                walls[nextWallIndex].enabled = true;
                nextWallIndex++;
                nextWallIndex %= maxWallCount;
                sprintf(nextWallIndexMinus1String, "%d / %d", nextWallIndex, maxWallCount);
            }
        }
        clickCount++;
    } else if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)) {
        Vector2 mousePos = GetMousePosition();
        for (int i = 0; i < maxWallCount; ++i) {
            if (walls[i].enabled && CheckCollisionPointRec(mousePos, walls[i].rect)) {
                walls[i].enabled = false;
                nextWallIndex--;
                sprintf(nextWallIndexMinus1String, "%d / %d", nextWallIndex, maxWallCount);
            }
        }
        int offset = 0;
        for (int i = 0; i < maxWallCount - 1; ++i) {
            if (walls[i].enabled && offset != 0) {
                walls[i - offset].enabled = walls[i].enabled;
                walls[i].enabled = false;
                walls[i - offset].rect.x = walls[i].rect.x;
                walls[i - offset].rect.y = walls[i].rect.y;
                walls[i - offset].rect.width = walls[i].rect.width;
                walls[i - offset].rect.height = walls[i].rect.height;
            } else if (!walls[i].enabled) {
                offset++;
            }
        }
        if (nextWallIndex < 0) {
            nextWallIndex = maxWallCount - 1;
            sprintf(nextWallIndexMinus1String, "%d / %d", nextWallIndex - 1, maxWallCount);
        }
    }

    // print level code to file
    if (IsKeyPressed(KEY_F1)) {
        char filename[64];

        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        sprintf(filename, "./%d-%02d-%02d_%02d:%02d:%02d.level", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

        FILE *fp = fopen(filename, "w");

        fprintf(fp, "\n\nlevels[%d].enabled = true;\n", levelIndex);
        for (int i = 0; i < maxWallCount; ++i) {
            if (!walls[i].enabled) continue;
            fprintf(
                    fp,
                    "levels[%d].walls[%d] = (struct Wall) {(Rectangle) {%f, %f, %f, %f}, true};\n", levelIndex, i,
                    walls[i].rect.x / screenWidth, walls[i].rect.y / screenHeight,
                    walls[i].rect.width / screenWidth, walls[i].rect.height / screenHeight
            );
        }
        fprintf(fp, "\n");
        fclose(fp);
    }

    // goto gameplay screen
    if (IsKeyPressed(KEY_F5)) gotoGameplayScreenLE = true;

    // reset level editor
    if (IsKeyPressed((KEY_F4))) {
        InitLevelEditorScreen();
        levelIndex++;
    }

    // load level
    if (IsKeyPressed(KEY_F2)) {
        levelIndex = (levelIndex + 1) % levelCount;
        int lastEnabledIndex = 0;
        for (int i = 0; i < maxWallCount; ++i) {
            walls[i] = levels[levelIndex].walls[i];
            if (levels[levelIndex].walls[i].enabled) lastEnabledIndex = i;
        }
        nextWallIndex = lastEnabledIndex + 1;
        sprintf(nextWallIndexMinus1String, "%d / %d", nextWallIndex - 1, maxWallCount);
    }
}

// this function handles drawing of all elements on the window
void DrawLevelEditorScreen(void) {
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), ColorFromHSV(180, 0.2, 0.2));

    DrawText(nextWallIndexMinus1String, 15, 15, 32, ColorAlpha(WHITE, 0.5));

    DrawRectangle((screenWidth * 0.7) / 2, 0, screenWidth * 0.3, 5, ColorFromHSV((int) (30 * GetTime()) % 360, 1, 1));
    DrawRectangle(0, (screenHeight * 0.7) / 2, 5, screenHeight * 0.3, ColorFromHSV((int) (30 * GetTime()) % 360, 1, 1));
    DrawRectangle(screenWidth - 5, (screenHeight * 0.7) / 2, 5, screenHeight * 0.3,
                  ColorFromHSV((int) (30 * GetTime()) % 360, 1, 1));
    DrawRectangle((screenWidth * 0.7) / 2, screenHeight - 5, screenWidth * 0.3, 5,
                  ColorFromHSV((int) (30 * GetTime()) % 360, 1, 1));

    for (int snapX = 0; snapX <= screenWidth / snapSize; ++snapX) {
        Color c = BLACK;
        if (snapX * snapSize == screenWidth / 2) c = WHITE;
        else if (snapX * snapSize % (screenWidth / 2) == screenWidth / 4) c = BLUE;
        else if (snapX * snapSize % (screenWidth / 4) == screenWidth / 8) c = GRAY;
        DrawLine(snapX * snapSize, 0, snapX * snapSize, screenHeight, c);
    }
    for (int snapY = 0; snapY <= screenHeight / snapSize; ++snapY) {
        Color c = BLACK;
        if (snapY * snapSize == screenHeight / 2) c = WHITE;
        else if (snapY * snapSize % (screenHeight / 2) == screenHeight / 4) c = BLUE;
        else if (snapY * snapSize % (screenHeight / 4) == screenHeight / 8) c = GRAY;
        DrawLine(0, snapY * snapSize, screenWidth, snapY * snapSize, c);
    }
    for (int i = 0; i < maxWallCount; ++i) {
        if (walls[i].enabled) {
            DrawRectangleRec(walls[i].rect, WHITE);
        }
    }
    for (int i = 0; i < playerCount; ++i) {
        DrawPlayerScore(&players[i]);
        DrawPlayer(&players[i]);
    }
}

bool GotoGameplayScreenFromLevelEditor() {
    return gotoGameplayScreenLE;
}
