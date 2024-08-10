//
// Created by igor on 2024.07.17..
//

#include <stdio.h>

#include "../constants.h"
#include "../screens.h"
#include "ui.h"

void InitButtonDefaults(struct Button *b, Rectangle rect, Color bgColor, Color fgColor, enum ButtonAction action, Font font, unsigned char fontSize, char *text) {
    b->rect = rect;
    b->bgColor = bgColor;
    b->fgColor = fgColor;
    b->action = action;
    b->spacing = 10;
    b->highlighted = false;
    SetButtonText(b, font, fontSize, text);
}

void SetButtonText(struct Button *b, Font font, unsigned char fontSize, char *text) {
    b->font = font;
    b->fontSize = fontSize;
    sprintf(b->text, "%s", text);
    b->textSize = MeasureTextEx(font, b->text, b->fontSize, b->spacing);
}

void DrawButton(struct Button *b) {
    int hue = (int) (hueRotationTimer + b->rect.x / screenWidth * 180 + b->rect.y / screenHeight * 180) % 360;
    DrawRectangleRec(b->rect, ColorFromHSV(hue, b->highlighted ? 0 : 1, 1));
    Vector2 textPos = (Vector2) {
            b->rect.x + (b->rect.width - b->textSize.x) / 2,
            b->rect.y + (b->rect.height - b->textSize.y) / 2
    };
    DrawTextEx(b->font, b->text, textPos, b->fontSize, b->spacing, ColorFromHSV(hue, 1, b->highlighted ? 1 : 0.25));
}

bool IsMouseOverButton(struct Button *b, Vector2 mousePos) {
    return CheckCollisionPointRec(mousePos, b->collideRect);
}

void ChangeValueAndClamp(float* value, float delta, float min, float max, short roundDecimalPlaces) {
    *value += delta;
    if (*value < min) *value = min;
    if (*value > max) *value = max;
    *value = (float)((int) (*value * (10.1 * roundDecimalPlaces))) / (10 * roundDecimalPlaces);
}

void UpdateSfxVolume() {
    for (int i = 0; i < sfxDoorOpenCount; ++i) SetSoundVolume(sfxDoorOpen[i], sfxDoorOpenVolume);
    for (int i = 0; i < sfxShootCount; ++i) SetSoundVolume(sfxShoot[i], sfxShootVolume);
    for (int i = 0; i < sfxDashCount; ++i) SetSoundVolume(sfxDash[i], sfxDashVolume);
    for (int i = 0; i < sfxDeadCount; ++i) SetSoundVolume(sfxDead[i], sfxDeadVolume);
    for (int i = 0; i < sfxHitCount; ++i) SetSoundVolume(sfxHit[i], sfxHitVolume);
}

void UpdateBgmVolume() {
    for (int i = 0; i < bgMusicCount; ++i) SetMusicVolume(bgMusic[i], bgMusicVolume);
    SetMusicVolume(startMusic, bgMusicVolume);
}

void InvokeButtonAction(struct Button *b) {
    float t;
    switch (b->action) {
        case NONE:
            break;
        case SAVE_SETTINGS_AND_PLAY:
            // this is a special action and therefore
            // should be handled at implementation level
            break;

        // INPUT MODES

        case SWITCH_TO_INPUT_KEYBOARD_ONLY:
            inputState = KEYBOARD_ONLY;
            break;
        case SWITCH_TO_INPUT_GAMEPAD_ONLY:
            inputState = GAMEPAD_ONLY;
            break;
        case SWITCH_TO_INPUT_MIXED:
            inputState = MIXED;
            break;

        // VOLUME CONTROL

        case INCREASE_MASTER_VOLUME:
            ChangeValueAndClamp(&masterVolume, 0.1f, 0, 1, 1);
            SetMasterVolume(masterVolume);
            break;
        case DECREASE_MASTER_VOLUME:
            ChangeValueAndClamp(&masterVolume, -0.1f, 0, 1, 1);
            SetMasterVolume(masterVolume);
            break;
        case INCREASE_SFX_VOLUME:
            t = 0.1f;
            ChangeValueAndClamp(&sfxShootVolume, t, 0, 1, 1);
            ChangeValueAndClamp(&sfxDashVolume, t, 0, 1, 1);
            ChangeValueAndClamp(&sfxDeadVolume, t, 0, 1, 1);
            ChangeValueAndClamp(&sfxDoorOpenVolume, t, 0, 1, 1);
            ChangeValueAndClamp(&sfxHitVolume, t, 0, 1, 1);
            UpdateSfxVolume();
            PlaySound(sfxDash[0]);
            break;
        case DECREASE_SFX_VOLUME:
            t = -0.1f;
            ChangeValueAndClamp(&sfxShootVolume, t, 0, 1, 1);
            ChangeValueAndClamp(&sfxDashVolume, t, 0, 1, 1);
            ChangeValueAndClamp(&sfxDeadVolume, t, 0, 1, 1);
            ChangeValueAndClamp(&sfxDoorOpenVolume, t, 0, 1, 1);
            ChangeValueAndClamp(&sfxHitVolume, t, 0, 1, 1);
            UpdateSfxVolume();
            break;
        case INCREASE_BGM_VOLUME:
            ChangeValueAndClamp(&bgMusicVolume, 0.1f, 0, 1, 1);
            UpdateBgmVolume();
            break;
        case DECREASE_BGM_VOLUME:
            ChangeValueAndClamp(&bgMusicVolume, -0.1f, 0, 1, 1);
            UpdateBgmVolume();
            break;

        // WINDOW AND DISPLAY

        case INCREASE_FINAL_RESOLUTION:
            // todo implement this, a list of possible resolutions
            //  needs to be made and switch between them here
            break;
        case DECREASE_FINAL_RESOLUTION:
            // see above
            break;
        case SWITCH_FULLSCREEN:
            // todo implement this
            break;
        case RANDOMIZE_EFFECTS_EVERY_ROUND:
            randomizeEffectsEveryRound = !randomizeEffectsEveryRound;
            break;

        // GAME SETTINGS

        case SWITCH_PLAYERS_KEEP_WEAPONS:
            playersKeepWeaponsBetweenRounds = !playersKeepWeaponsBetweenRounds;
            break;
        case INCREASE_PLAYER_EFFECT_MULTIPLIER:
            ChangeValueAndClamp(&playerEffectMultiplier, 0.1f, 0, 2, 1);
            break;
        case DECREASE_PLAYER_EFFECT_MULTIPLIER:
            ChangeValueAndClamp(&playerEffectMultiplier, -0.1f, 0, 2, 1);
            break;
        case INCREASE_WINS_NEEDED_TO_WIN_GAME:
            t = (float)winsNeededToWinGame;
            ChangeValueAndClamp(&t, 1, 2, 25, 1);
            winsNeededToWinGame = (int)t;
            break;
        case DECREASE_WINS_NEEDED_TO_WIN_GAME:
            t = (float)winsNeededToWinGame;
            ChangeValueAndClamp(&t, -1, 2, 25, 1);
            winsNeededToWinGame = (int)t;
            break;
        case INCREASE_RECOIL_SCALE:
            ChangeValueAndClamp(&recoilScale, 0.1f, 0, 2, 1);
            break;
        case DECREASE_RECOIL_SCALE:
            ChangeValueAndClamp(&recoilScale, -0.1f, 0, 2, 1);
            break;
        default:
            perror("Invalid button action.");
    }
}

void InvokeButtonUpdate(struct Button *b) {
    switch (b->action) {
        case SWITCH_TO_INPUT_KEYBOARD_ONLY:
            b->highlighted = inputState == KEYBOARD_ONLY;
            break;
        case SWITCH_TO_INPUT_GAMEPAD_ONLY:
            b->highlighted = inputState == GAMEPAD_ONLY;
            break;
        case SWITCH_TO_INPUT_MIXED:
            b->highlighted = inputState == MIXED;
            break;
        case SWITCH_FULLSCREEN:
            b->highlighted = IsWindowFullscreen();
            break;
        case RANDOMIZE_EFFECTS_EVERY_ROUND:
            b->highlighted = randomizeEffectsEveryRound;
            break;
        case SWITCH_PLAYERS_KEEP_WEAPONS:
            b->highlighted = playersKeepWeaponsBetweenRounds;
            break;
        default:
            break;
    }
}
