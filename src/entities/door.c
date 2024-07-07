//
// Created by igor on 2024.07.06..
//

#include "door.h"
#include "playerEffect.h"

#include <stdlib.h>
#include <stdio.h>

void InitDoorsWithRandomEffect(struct Door *d) {
    d->animationOpenTime = 1;
    d->color = ColorFromHSV(30 * (rand() % 12), 1, .5);
    if (rand() % 100 < 50) {
        d->isDebuff = false;
        d->playerEffect = getRandomBuff();
    } else {
        d->isDebuff = true;
        d->playerEffect = getRandomDebuff();
    }
    switch (d->location) {
        case TOP:
            d->finalRect.height = 5;
            d->finalRect.width = screenWidth * 0.3;
            d->finalRect.x = (screenWidth - d->finalRect.width) / 2;
            d->finalRect.y = 0;
            break;
        case LEFT:
            d->finalRect.height = screenHeight * 0.2;
            d->finalRect.width = 5;
            d->finalRect.x = 0;
            d->finalRect.y = (screenHeight - d->finalRect.height) / 2;
            break;
        case RIGHT:
            d->finalRect.height = screenHeight * 0.2;
            d->finalRect.width = 5;
            d->finalRect.x = screenWidth - d->finalRect.width;
            d->finalRect.y = (screenHeight - d->finalRect.height) / 2;
            break;
        case BOTTOM:
            d->finalRect.height = 5;
            d->finalRect.width = screenWidth * 0.3;
            d->finalRect.x = (screenWidth - d->finalRect.width) / 2;
            d->finalRect.y = screenHeight - d->finalRect.height;
            break;
        default:
            break;
    }
}

void DrawDoor(struct Door *d) {
    DrawRectangle(d->finalRect.x, d->finalRect.y, d->finalRect.width, d->finalRect.height, d->color);
}
