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

    switch (d->location) {
        case TOP:
            d->finalRect.height = 5;
            d->finalRect.width = screenWidth * 0.3;
            d->finalRect.x = (screenWidth - d->finalRect.width) / 2;
            d->finalRect.y = 0;
            d->playerEffectStringPositionX = d->finalRect.x + d->finalRect.width / 2 - textWidth;
            d->playerEffectStringPositionY = 15;
            break;
        case LEFT:
            d->finalRect.height = screenHeight * 0.3;
            d->finalRect.width = 5;
            d->finalRect.x = 0;
            d->finalRect.y = (screenHeight - d->finalRect.height) / 2;
            d->playerEffectStringPositionX = 15;
            d->playerEffectStringPositionY = screenHeight / 2 - doorFontSize / 2;
            break;
        case RIGHT:
            d->finalRect.height = screenHeight * 0.3;
            d->finalRect.width = 5;
            d->finalRect.x = screenWidth - d->finalRect.width;
            d->finalRect.y = (screenHeight - d->finalRect.height) / 2;
            d->playerEffectStringPositionX = screenWidth - textWidth - 15;
            d->playerEffectStringPositionY = screenHeight / 2 - doorFontSize / 2;
            break;
        case BOTTOM:
            d->finalRect.height = 5;
            d->finalRect.width = screenWidth * 0.3;
            d->finalRect.x = (screenWidth - d->finalRect.width) / 2;
            d->finalRect.y = screenHeight - d->finalRect.height;
            d->playerEffectStringPositionX = d->finalRect.x + d->finalRect.width / 2 - textWidth;
            d->playerEffectStringPositionY = screenHeight - 15 - doorFontSize;
            break;
        default:
            break;
    }
}

void DrawDoor(struct Door *d) {
    float percentageOpen = sqrtf((GetTime() - d->timeSpawned) / d->animationOpenTime);
    if (percentageOpen > 1) percentageOpen = 1;
    switch (d->location) {
        case TOP:
            DrawRectangle(d->finalRect.x, d->finalRect.y, d->finalRect.width * percentageOpen, d->finalRect.height, d->color);
            break;
        case LEFT:
            DrawRectangle(d->finalRect.x, d->finalRect.y, d->finalRect.width, d->finalRect.height * percentageOpen, d->color);
            break;
        case RIGHT:
            DrawRectangle(d->finalRect.x, d->finalRect.y, d->finalRect.width, d->finalRect.height * percentageOpen, d->color);
            break;
        case BOTTOM:
            DrawRectangle(d->finalRect.x, d->finalRect.y, d->finalRect.width * percentageOpen, d->finalRect.height, d->color);
            break;
        default:
            break;
    }
    DrawText(d->playerEffectString, d->playerEffectStringPositionX, d->playerEffectStringPositionY, doorFontSize, d->color);
}
