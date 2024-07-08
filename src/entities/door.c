//
// Created by igor on 2024.07.06..
//

#include "door.h"
#include "playerEffect.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

void InitDoorsWithRandomEffect(struct Door *d) {
    d->animationOpenTime = 1;

    if (rand() % 100 < 50) {
        d->isDebuff = false;
        d->playerEffect = getRandomBuff();
    } else {
        d->isDebuff = true;
        d->playerEffect = getRandomDebuff();
    }

    d->color = ColorFromHSV((360 / playerEffectCount * d->playerEffect) % 360, .9, .9);

    sprintf(d->playerEffectString, "%d", d->playerEffect);
    int textWidth = MeasureText(d->playerEffectString, doorFontSize);
    int padding = 50;
    switch (d->location) {
        case TOP:
            d->finalRect.height = 5;
            d->finalRect.width = screenWidth * 0.3;
            d->finalRect.x = (screenWidth - d->finalRect.width) / 2;
            d->finalRect.y = 0;
            d->playerEffectStringPositionX = d->finalRect.x + d->finalRect.width / 2 - textWidth;
            d->playerEffectStringPositionY = padding;
            break;
        case LEFT:
            d->finalRect.height = screenHeight * 0.3;
            d->finalRect.width = 5;
            d->finalRect.x = 0;
            d->finalRect.y = (screenHeight - d->finalRect.height) / 2;
            d->playerEffectStringPositionX = padding * 1.5;
            d->playerEffectStringPositionY = screenHeight / 2 - doorFontSize / 2;
            break;
        case RIGHT:
            d->finalRect.height = screenHeight * 0.3;
            d->finalRect.width = 5;
            d->finalRect.x = screenWidth - d->finalRect.width;
            d->finalRect.y = (screenHeight - d->finalRect.height) / 2;
            d->playerEffectStringPositionX = screenWidth - textWidth - padding * 1.5;
            d->playerEffectStringPositionY = screenHeight / 2 - doorFontSize / 2;
            break;
        case BOTTOM:
            d->finalRect.height = 5;
            d->finalRect.width = screenWidth * 0.3;
            d->finalRect.x = (screenWidth - d->finalRect.width) / 2;
            d->finalRect.y = screenHeight - d->finalRect.height;
            d->playerEffectStringPositionX = d->finalRect.x + d->finalRect.width / 2 - textWidth;
            d->playerEffectStringPositionY = screenHeight - padding - doorFontSize;
            break;
        default:
            break;
    }
}

void DrawDoor(struct Door *d) {
    float percentageOpen = sqrtf((GetTime() - d->timeSpawned) / d->animationOpenTime);
    if (percentageOpen > 1) percentageOpen = 1;
    int multiply = 15;
    float alpha = 0.8;
    switch (d->location) {
        case TOP:
            DrawRectangleGradientV(d->finalRect.x, d->finalRect.y, d->finalRect.width * percentageOpen, d->finalRect.height * multiply, ColorAlpha(d->color, alpha), BLANK);
            break;
        case LEFT:
            DrawRectangleGradientH(d->finalRect.x, d->finalRect.y, d->finalRect.width * multiply, d->finalRect.height * percentageOpen, ColorAlpha(d->color, alpha), BLANK);
            break;
        case RIGHT:
            DrawRectangleGradientH(d->finalRect.x - d->finalRect.width * multiply, d->finalRect.y, d->finalRect.width * (multiply + 1), d->finalRect.height * percentageOpen, BLANK, ColorAlpha(d->color, alpha));
            break;
        case BOTTOM:
            DrawRectangleGradientV(d->finalRect.x, d->finalRect.y - d->finalRect.height * (multiply + 1), d->finalRect.width * percentageOpen, d->finalRect.height * multiply, BLANK, ColorAlpha(d->color, alpha));
            break;
        default:
            break;
    }
    DrawText(d->playerEffectString, d->playerEffectStringPositionX, d->playerEffectStringPositionY, doorFontSize, d->color);
}
