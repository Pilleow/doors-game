//
// Created by igor on 2024.07.17..
//

#include "raylib.h"

#ifndef DOORS_GAME_UI_H
#define DOORS_GAME_UI_H

enum ButtonAction {
    // CONTROL

    NONE,
    SAVE_SETTINGS_AND_PLAY,

    // INPUT MODES

    SWITCH_TO_INPUT_KEYBOARD_ONLY,
    SWITCH_TO_INPUT_GAMEPAD_ONLY,
    SWITCH_TO_INPUT_MIXED,

    // VOLUME CONTROL

    INCREASE_MASTER_VOLUME,
    DECREASE_MASTER_VOLUME,
    INCREASE_SFX_VOLUME,
    DECREASE_SFX_VOLUME,
    INCREASE_BGM_VOLUME,
    DECREASE_BGM_VOLUME,

    // WINDOW AND DISPLAY

    INCREASE_FINAL_RESOLUTION,
    DECREASE_FINAL_RESOLUTION,
    SWITCH_FULLSCREEN,
    SHOW_FPS,

    // GAME SETTINGS

    SWITCH_ALL_PLAYERS_USING_THE_SAME_WEAPON,
    INCREASE_PLAYER_EFFECT_MULTIPLIER,
    DECREASE_PLAYER_EFFECT_MULTIPLIER,
    INCREASE_WINS_NEEDED_TO_WIN_GAME,
    DECREASE_WINS_NEEDED_TO_WIN_GAME,
    INCREASE_RECOIL_SCALE,
    DECREASE_RECOIL_SCALE,
};

struct Button {
    Rectangle rect;
    Rectangle collideRect;
    Color bgColor;
    Color fgColor;
    char text[32];
    Vector2 textSize;
    Font font;
    int fontSize;
    int spacing;
    bool highlighted;
    enum ButtonAction action;
};

void InitButtonDefaults(struct Button *b, Rectangle rect, Color bgColor, Color fgColor, enum ButtonAction action, Font font, unsigned char fontSize, char *text);

void SetButtonText(struct Button *b, Font font, unsigned char fontSize, char *text);

void DrawButton(struct Button *b);

bool IsMouseOverButton(struct Button *b, Vector2 mousePos);

void InvokeButtonAction(struct Button *b);

void InvokeButtonUpdate(struct Button *b);

#endif //DOORS_GAME_UI_H
