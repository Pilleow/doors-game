#include <stdio.h>
#include "raylib.h"
#include "raymath.h"
#include "screens.h"
#include "constants.h"

#include "entities/ui.h"

// local variable declaration below ------------------------------------------------------------------------------------

bool gotoGameplayScreenMM = false;
struct Button buttons[mainMenuButtonCount];
char masterVolumeText[8];
int masterVolumeTextX = 0;
char bgVolumeText[8];
int bgVolumeTextX = 0;
char sfxVolumeText[8];
int sfxVolumeTextX = 0;
char pEffText[8];
int pEffTextX = 0;
char recoilText[8];
int recoilTextX = 0;
char winsText[8];
int winsTextX = 0;

float warningPopupAnimStart = -100;
bool warningActive = false;

// functions definition below ------------------------------------------------------------------------------------------

void _UpdateText(void);

void InitMainMenuScreen(void) {
    ShowCursor();
    gotoGameplayScreenMM = false;

    for (int i = 0; i < mainMenuButtonCount; ++i) {
        InitButtonDefaults(
                &buttons[i], (Rectangle) {-10, -10, 1, 1}, BLACK, BLACK,
                NONE, GetFontDefault(), 1, ""
        );
    }

    InitButtonDefaults(
            &buttons[0], (Rectangle) {screenWidth / 2 - 196, screenHeight / 2 - 50, 394, 100},
            GREEN, WHITE, SAVE_SETTINGS_AND_PLAY, GetFontDefault(), 50, "PLAY"
    );
    InitButtonDefaults(
            &buttons[1], (Rectangle) {100, screenHeight / 2 - 35, 100, 70},
            GREEN, WHITE, INCREASE_BGM_VOLUME, GetFontDefault(), 40, "B+"
    );
    InitButtonDefaults(
            &buttons[2], (Rectangle) {400, screenHeight / 2 - 35, 100, 70},
            GREEN, WHITE, DECREASE_BGM_VOLUME, GetFontDefault(), 40, "B-"
    );
    InitButtonDefaults(
            &buttons[3], (Rectangle) {100, screenHeight / 2 - 35 - 150, 100, 70},
            GREEN, WHITE, INCREASE_MASTER_VOLUME, GetFontDefault(), 40, "M+"
    );
    InitButtonDefaults(
            &buttons[4], (Rectangle) {400, screenHeight / 2 - 35 - 150, 100, 70},
            GREEN, WHITE, DECREASE_MASTER_VOLUME, GetFontDefault(), 40, "M-"
    );
    InitButtonDefaults(
            &buttons[5], (Rectangle) {100, screenHeight / 2 - 35 + 150, 100, 70},
            GREEN, WHITE, INCREASE_SFX_VOLUME, GetFontDefault(), 40, "S+"
    );
    InitButtonDefaults(
            &buttons[6], (Rectangle) {400, screenHeight / 2 - 35 + 150, 100, 70},
            GREEN, WHITE, DECREASE_SFX_VOLUME, GetFontDefault(), 40, "S-"
    );
    InitButtonDefaults(
            &buttons[7], (Rectangle) {300 - 175, screenHeight / 2 - 35 + 300, 350, 70},
            GREEN, WHITE, SWITCH_ALL_PLAYERS_USING_THE_SAME_WEAPON, GetFontDefault(), 40, "SAME GUNS"
    );
    InitButtonDefaults(
            &buttons[8], (Rectangle) {screenWidth - 100 - 400, screenHeight / 2 - 35, 100, 70},
            GREEN, WHITE, INCREASE_RECOIL_SCALE, GetFontDefault(), 40, "R+"
    );
    InitButtonDefaults(
            &buttons[9], (Rectangle) {screenWidth - 100 - 100, screenHeight / 2 - 35, 100, 70},
            GREEN, WHITE, DECREASE_RECOIL_SCALE, GetFontDefault(), 40, "R-"
    );
    InitButtonDefaults(
            &buttons[10], (Rectangle) {screenWidth - 100 - 400, screenHeight / 2 - 35 - 150, 100, 70},
            GREEN, WHITE, INCREASE_PLAYER_EFFECT_MULTIPLIER, GetFontDefault(), 40, "E+"
    );
    InitButtonDefaults(
            &buttons[11], (Rectangle) {screenWidth - 100 - 100, screenHeight / 2 - 35 - 150, 100, 70},
            GREEN, WHITE, DECREASE_PLAYER_EFFECT_MULTIPLIER, GetFontDefault(), 40, "E-"
    );
    InitButtonDefaults(
            &buttons[12], (Rectangle) {screenWidth - 100 - 400, screenHeight / 2 - 35 + 150, 100, 70},
            GREEN, WHITE, INCREASE_WINS_NEEDED_TO_WIN_GAME, GetFontDefault(), 40, "W+"
    );
    InitButtonDefaults(
            &buttons[13], (Rectangle) {screenWidth - 100 - 100, screenHeight / 2 - 35 + 150, 100, 70},
            GREEN, WHITE, DECREASE_WINS_NEEDED_TO_WIN_GAME, GetFontDefault(), 40, "W-"
    );
    InitButtonDefaults(
            &buttons[14], (Rectangle) {screenWidth - 300 - 175, screenHeight / 2 - 35 + 300, 350, 70},
            GREEN, WHITE, SHOW_FPS, GetFontDefault(), 40, "SHOW FPS"
    );
    InitButtonDefaults(
            &buttons[15], (Rectangle) {screenWidth / 2 - 250, screenHeight / 2 + 300, 100, 70},
            GREEN, WHITE, SWITCH_TO_INPUT_KEYBOARD_ONLY, GetFontDefault(), 40, "KB"
    );
    InitButtonDefaults(
            &buttons[16], (Rectangle) {screenWidth / 2 - 50, screenHeight / 2 + 300, 100, 70},
            GREEN, WHITE, SWITCH_TO_INPUT_MIXED, GetFontDefault(), 40, "MX"
    );
    InitButtonDefaults(
            &buttons[17], (Rectangle) {screenWidth / 2 + 150, screenHeight / 2 + 300, 100, 70},
            GREEN, WHITE, SWITCH_TO_INPUT_GAMEPAD_ONLY, GetFontDefault(), 40, "GP"
    );

    int monitor = GetCurrentMonitor();
    int fw = GetMonitorWidth(monitor);
    int fh = GetMonitorHeight(monitor);
    for (int i = 0; i < mainMenuButtonCount; ++i) {
        buttons[i].collideRect = (Rectangle) {
                buttons[i].rect.x / screenWidth * fw,
                buttons[i].rect.y / screenHeight * fh,
                buttons[i].rect.width / screenWidth * fw,
                buttons[i].rect.height / screenHeight * fh
        };
        InvokeButtonUpdate(&buttons[i]);
    }
    _UpdateText();
}

void _UpdateText(void) {
    char format[] = "%d%%";

    sprintf(masterVolumeText, format, (int) (masterVolume * 100));
    masterVolumeTextX = 300 - MeasureText(masterVolumeText, 40) / 2;
    sprintf(bgVolumeText, format, (int) (bgMusicVolume * 100));
    bgVolumeTextX = 300 - MeasureText(bgVolumeText, 40) / 2;
    sprintf(sfxVolumeText, format, (int) (sfxShootVolume * 100));
    sfxVolumeTextX = 300 - MeasureText(sfxVolumeText, 40) / 2;

    sprintf(pEffText, format, (int) (playerEffectMultiplier * 100));
    pEffTextX = screenWidth - 300 - MeasureText(pEffText, 40) / 2;
    sprintf(recoilText, format, (int) (recoilScale * 100));
    recoilTextX = screenWidth - 300 - MeasureText(recoilText, 40) / 2;
    sprintf(winsText, "%d", (int) (winsNeededToWinGame));
    winsTextX = screenWidth - 300 - MeasureText(winsText, 40) / 2;
}

void UpdateMainMenuScreen(void) {
    hueRotationTimer += GetFrameTime() * hueRotationSpeed;
    if (hueRotationSpeed > defaultHueRotationSpeed) {
        hueRotationSpeed = defaultHueRotationSpeed + 0.96 * (hueRotationSpeed - defaultHueRotationSpeed);
    }
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        hueRotationSpeed = defaultHueRotationSpeed * hueRotationSpeedOnHitMultiplier;
        Vector2 mousePos = GetMousePosition();
        for (int i = 0; i < mainMenuButtonCount; ++i) {
            if (IsMouseOverButton(&buttons[i], mousePos)) {
                if (buttons[i].action == SAVE_SETTINGS_AND_PLAY) {
                    gotoGameplayScreenMM = true;
                    SeekMusicStream(startMusic, 1);
                } else {
                    InvokeButtonAction(&buttons[i]);
                }
                hueRotationSpeed = defaultHueRotationSpeed * hueRotationSpeedOnDeathMultiplier;
            }
        }
        for (int i = 0; i < mainMenuButtonCount; ++i) {
            InvokeButtonUpdate(&buttons[i]);
        }
        _UpdateText();

        if (!warningActive && (playerEffectMultiplier > 1.69 || recoilScale > 1)) {
            warningActive = true;
            warningPopupAnimStart = GetTime();
        } else if (warningActive && playerEffectMultiplier <= 1.69 && recoilScale <= 1) {
            warningActive = false;
            warningPopupAnimStart = GetTime();
        }
    }

    if (IsKeyPressed(KEY_SPACE)) {
        gotoGameplayScreenMM = true;
    }
}

void DrawMainMenuScreen(void) {
    DrawTexture(texBackground, 0, 0, WHITE);
    for (int i = 0; i < mainMenuButtonCount; ++i) {
        DrawButton(&buttons[i]);
    }
    DrawText(masterVolumeText, masterVolumeTextX, screenHeight / 2 - 170, 40, WHITE);
    DrawText(bgVolumeText, bgVolumeTextX, screenHeight / 2 - 20, 40, WHITE);
    DrawText(sfxVolumeText, sfxVolumeTextX, screenHeight / 2 + 130, 40, WHITE);
    DrawText(pEffText, pEffTextX, screenHeight / 2 - 170, 40, WHITE);
    DrawText(recoilText, recoilTextX, screenHeight / 2 - 20, 40, WHITE);
    DrawText(winsText, winsTextX, screenHeight / 2 + 130, 40, WHITE);

    DrawTexturePro(
            texMainMenu,
            (Rectangle) {0, 0, 1920 / 4, 1080 / 4},
            (Rectangle) {0, 0, screenWidth, screenHeight},
            (Vector2) {0, 0},
            0,
            WHITE
    );

    float t = GetTime() - warningPopupAnimStart;
    if (warningActive) {
        int x = screenWidth - 373;
        if (t < 2.0f) x += 373.0f / (1.0f + 373.0f * t) - 0.5f;
        DrawTexture(texGoofyWarning, x, screenHeight / 10, WHITE);
    }
    else if (!warningActive && t < 4.0f) {
        int x = screenWidth + 4;
        if (t < 2.0f) x -= 400.0f / (1.0f + 400.0f * t) - 0.5f;
        DrawTexture(texGoofyWarning, x, screenHeight / 10, WHITE);
    }
}

bool GotoGameplayScreenFromMainMenu() {
    return gotoGameplayScreenMM;
}
