//
// Created by igor on 2024.07.06..
//

#include "door.h"
#include "playerEffect.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

void InitDoorsWithRandomEffect(struct Door *d) {
    d->animationOpenTime = 60;

    if (randomizeEffectsEveryRound) {
        d->isDebuff = false;
        d->playerEffect = RANDOM_EFFECT_TO_EVERYONE;
    } else {
        float chance = rand() % 100;
        if (chance < 40) {
            d->isDebuff = false;
            d->playerEffect = getRandomBuff();
        } else if (chance < 80) {
            d->isDebuff = true;
            d->playerEffect = getRandomDebuff();
        } else {
            d->isDebuff = false;
            d->playerEffect = getRandomSpecialEffect();
        }
    }

    d->color = ColorFromHSV((360 / playerEffectCount * d->playerEffect) % 360, .9, .9);

    sprintf(d->playerEffectString, "%s", playerEffectNames[d->playerEffect]);
    d->playerEffectsStringWidth = MeasureTextEx(GetFontDefault(), d->playerEffectString, doorFontSize, 10).x;
    int padding = 75;
    switch (d->location) {
        case TOP:
            d->finalRect.height = 5;
            d->finalRect.width = screenWidth * 0.3;
            d->finalRect.x = (screenWidth - d->finalRect.width) / 2;
            d->finalRect.y = 0;
            d->playerEffectStringPositionX = d->finalRect.x + (d->finalRect.width - d->playerEffectsStringWidth) / 2;
            d->playerEffectStringPositionY = padding;
            break;
        case LEFT:
            d->finalRect.height = screenHeight * 0.3;
            d->finalRect.width = 5;
            d->finalRect.x = 0;
            d->finalRect.y = (screenHeight - d->finalRect.height) / 2;
            d->playerEffectStringPositionX = padding;
            d->playerEffectStringPositionY = screenHeight / 2 - doorFontSize / 2;
            break;
        case RIGHT:
            d->finalRect.height = screenHeight * 0.3;
            d->finalRect.width = 5;
            d->finalRect.x = screenWidth - d->finalRect.width;
            d->finalRect.y = (screenHeight - d->finalRect.height) / 2;
            d->playerEffectStringPositionX = screenWidth - d->playerEffectsStringWidth - padding;
            d->playerEffectStringPositionY = screenHeight / 2 - doorFontSize / 2;
            break;
        case BOTTOM:
            d->finalRect.height = 5;
            d->finalRect.width = screenWidth * 0.3;
            d->finalRect.x = (screenWidth - d->finalRect.width) / 2;
            d->finalRect.y = screenHeight - d->finalRect.height;
            d->playerEffectStringPositionX = d->finalRect.x + (d->finalRect.width - d->playerEffectsStringWidth) / 2;
            d->playerEffectStringPositionY = screenHeight - padding - doorFontSize;
            break;
        default:
            break;
    }
}

float _functionDrawDoorCalculate(struct Door *d, float x) {
    return 1 - 1 / ((d->animationOpenTime * x * x * x) + 1);
}

void DrawDoor(struct Door *d) {
    float percentageSlideIn, percentageOpen, t, h;
    t = GetTime() - d->timeSpawned;
    h = 2.5 - log10(2.5 * d->animationOpenTime);
    percentageOpen = _functionDrawDoorCalculate(d, t);
    if (t > h) percentageSlideIn = _functionDrawDoorCalculate(d, t - h);
    else percentageSlideIn = 0;

    int x, y;
    int multiply = 15;
    float alpha = 0.8;
    float rotation = 3 * sinf(3 * GetTime());
    switch (d->location) {
        case TOP:
            x = d->playerEffectStringPositionX;
            y = d->playerEffectStringPositionY - 3 * doorFontSize * (1 - percentageSlideIn);
            DrawRectangleGradientV(d->finalRect.x, d->finalRect.y, d->finalRect.width * percentageOpen,
                                   d->finalRect.height * multiply, ColorAlpha(d->color, alpha), BLANK);
            break;
        case LEFT:
            x = d->playerEffectStringPositionX - 3 * d->playerEffectsStringWidth * (1 - percentageSlideIn);
            y = d->playerEffectStringPositionY;
            DrawRectangleGradientH(d->finalRect.x, d->finalRect.y, d->finalRect.width * multiply,
                                   d->finalRect.height * percentageOpen, ColorAlpha(d->color, alpha), BLANK);
            break;
        case RIGHT:
            x = d->playerEffectStringPositionX + 3 * d->playerEffectsStringWidth * (1 - percentageSlideIn);
            y = d->playerEffectStringPositionY;
            DrawRectangleGradientH(d->finalRect.x - d->finalRect.width * multiply, d->finalRect.y,
                                   d->finalRect.width * (multiply + 1), d->finalRect.height * percentageOpen, BLANK,
                                   ColorAlpha(d->color, alpha));
            break;
        case BOTTOM:
            x = d->playerEffectStringPositionX;
            y = d->playerEffectStringPositionY + 3 * doorFontSize * (1 - percentageSlideIn);
            DrawRectangleGradientV(d->finalRect.x, d->finalRect.y - d->finalRect.height * (multiply - 1),
                                   d->finalRect.width * percentageOpen, d->finalRect.height * multiply, BLANK,
                                   ColorAlpha(d->color, alpha));
            break;
        default:
            return;
    }
    x += d->playerEffectsStringWidth / 2;
    y += doorFontSize / 2;
    DrawTextPro(GetFontDefault(), d->playerEffectString, (Vector2) {x, y},
                (Vector2) {d->playerEffectsStringWidth / 2, doorFontSize / 2}, rotation, doorFontSize, 10, d->color);
}
