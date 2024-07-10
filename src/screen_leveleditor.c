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

static bool gotoGameplayScreen = false;
int snapSize = 1;
int levelIndex = levelCount;
int nextWallIndex = 0;
int clickCount = 0;
struct Wall walls[maxWallCount];

// functions definition below ------------------------------------------------------------------------------------------

// this function initializes the gameplay screen
void InitLevelEditorScreen(void) {
    clickCount = 0;
    nextWallIndex = 0;
    gotoGameplayScreen = false;
    snapSize = screenWidth / screenHeight * 30;
    for (int i = 0; i < maxWallCount; ++i) walls[i].enabled = false;
}

// this function updates the game regardless of current gameState
void UpdateLevelEditorScreen(void) {
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        Vector2 mousePos = GetMousePosition();
        mousePos.x = ((int) (mousePos.x / snapSize)) * snapSize;
        mousePos.y = ((int) (mousePos.y / snapSize)) * snapSize;
        if (clickCount % 2 == 0) {
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

            walls[nextWallIndex].enabled = true;
            nextWallIndex++;
            nextWallIndex %= maxWallCount;
        }
        clickCount++;
    }
    else if (IsKeyReleased(KEY_DELETE)) {
        Vector2 mousePos = GetMousePosition();
        for (int i = 0; i < maxWallCount; ++i) {
            if (CheckCollisionPointRec(mousePos, walls[i].rect)) {
                walls[i].enabled = false;
            }
        }
        for (int i = 0; i < maxWallCount - 1; ++i) {
            if (!walls[i].enabled) {
                walls[i].enabled = walls[i + 1].enabled;
                walls[i + 1].enabled = false;
                if (!walls[i].enabled) continue;
                walls[i].rect.x = walls[i + 1].rect.x;
                walls[i].rect.y = walls[i + 1].rect.y;
                walls[i].rect.width = walls[i + 1].rect.width;
                walls[i].rect.height = walls[i + 1].rect.height;
            }
        }
        nextWallIndex--;
        if (nextWallIndex < 0) nextWallIndex += maxWallCount;
    }

    // print level code to stdout
    if (IsKeyPressed(KEY_F1)) {
        printf("\n\nlevels[%d].enabled = true;\n", levelIndex);
        for (int i = 0; i < maxWallCount; ++i) {
            if (!walls[i].enabled) continue;
            printf(
                    "levels[%d].walls[%d] = (struct Wall) {(Rectangle) {%f, %f, %f, %f}, true};\n", levelIndex, i,
                    walls[i].rect.x / screenWidth, walls[i].rect.y / screenHeight,
                    walls[i].rect.width / screenWidth, walls[i].rect.height / screenHeight
            );
        }
        printf("\n");
    }

    // goto gameplay screen
    if (IsKeyPressed(KEY_F5)) gotoGameplayScreen = true;

    // reset level editor
    if (IsKeyPressed((KEY_F4))) {
        InitLevelEditorScreen();
        levelIndex++;
    }
}

// this function handles drawing of all elements on the window
void DrawLevelEditorScreen(void) {
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), ColorFromHSV(180, 0.2, 0.2));

    DrawRectangle((screenWidth * 0.7) / 2, 0, screenWidth * 0.3, 5, ColorFromHSV((int)(30 * GetTime()) % 360, 1, 1));
    DrawRectangle(0, (screenHeight * 0.7) / 2, 5, screenHeight * 0.3, ColorFromHSV((int)(30 * GetTime()) % 360, 1, 1));
    DrawRectangle(screenWidth - 5, (screenHeight * 0.7) / 2, 5, screenHeight * 0.3, ColorFromHSV((int)(30 * GetTime()) % 360, 1, 1));
    DrawRectangle((screenWidth * 0.7) / 2, screenHeight - 5, screenWidth * 0.3, 5, ColorFromHSV((int)(30 * GetTime()) % 360, 1, 1));

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
}

bool GotoGameplayScreen() {
    return gotoGameplayScreen;
}