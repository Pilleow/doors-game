//
// Created by igor on 2024.07.09..
//

#include <stdio.h>
#include <stdlib.h>

#include "wall.h"
#include "../constants.h"

bool CheckWallBooletCollisionAndFixPosition(struct Wall *w, struct Boolet *e) {
    if (!CheckCollisionRecs(w->rect, e->rect)) return false;
    if (e->type == BOUNCING) {
        struct Vector2 eCenter = (Vector2) {
                e->rect.x + e->rect.width / 2,
                e->rect.y + e->rect.height / 2,
        };

        int dx, dy;

        if (eCenter.y < w->rect.y + w->rect.height / 2) dy = fabsf(eCenter.y - w->rect.y);
        else dy = fabsf(eCenter.y - w->rect.y - w->rect.height);

        if (eCenter.x < w->rect.x + w->rect.width / 2) dx = fabsf(eCenter.x - w->rect.x);
        else dx = fabsf(eCenter.x - w->rect.x - w->rect.width);

        if (dx < dy) e->velocity.x *= -1;
        else e->velocity.y *= -1;
    } else if (e->type != EXPLODING) e->enabled = false;
    return true;
}

bool FixEntityPosition(struct Wall *w, Rectangle *rect) {
    struct Vector2 eCenter = (Vector2) {
            rect->x + rect->width / 2,
            rect->y + rect->height / 2,
    };

    bool isOnlyOneCornerTouching = false;
    struct Vector2 corner;
    int cornerIndex;

    for (int i = 0; i < 4; ++i) {
        struct Vector2 tempCorner = (Vector2) {
                w->rect.x + w->rect.width * (i / 2),
                w->rect.y + w->rect.height * (i % 2)
        };
        if (!CheckCollisionPointRec(tempCorner, *rect)) continue;
        if (isOnlyOneCornerTouching == true) {
            isOnlyOneCornerTouching = false;
            break;
        }
        isOnlyOneCornerTouching = true;
        corner = tempCorner;
        cornerIndex = i;
    }

    if (isOnlyOneCornerTouching) {
        int dx = 0, dy = 0;
        struct Vector2 eCorner;

        for (int j = 0; j < 4; ++j) {
            eCorner = (Vector2) {
                    rect->x + rect->width * (j / 2),
                    rect->y + rect->height * (j % 2)
            };
            if (!CheckCollisionPointRec(eCorner, w->rect)) continue;
            dx = fabsf(eCorner.x - corner.x);
            dy = fabsf(eCorner.y - corner.y);
            break;
        }

        if (dx > dy) {
            switch (cornerIndex) {
                case 0: // top left
                    rect->y = w->rect.y - rect->height;
                    break;
                case 1: // bottom left
                    rect->y = w->rect.y + w->rect.height;
                    break;
                case 2: // top right
                    rect->y = w->rect.y - rect->height;
                    break;
                case 3: // bottom right
                    rect->y = w->rect.y + w->rect.height;
                    break;
            }
        } else if (dx < dy) {
            switch (cornerIndex) {
                case 0: // top left
                    rect->x = w->rect.x - rect->width;
                    break;
                case 1: // bottom left
                    rect->x = w->rect.x - rect->width;
                    break;
                case 2: // top right
                    rect->x = w->rect.x + w->rect.width;
                    break;
                case 3: // bottom right
                    rect->x = w->rect.x + w->rect.width;
                    break;
            }
        }
    } else {
        if (eCenter.x > w->rect.x && eCenter.x < w->rect.x + w->rect.width) {
            if (eCenter.y < w->rect.y + w->rect.height / 2) rect->y = w->rect.y - rect->height;
            else rect->y = w->rect.y + w->rect.height;
        } else {
            if (eCenter.x < w->rect.x + w->rect.width / 2) rect->x = w->rect.x - rect->width;
            else rect->x = w->rect.x + w->rect.width;
        }
    }
    return true;
}

void DrawWall(struct Wall *w) {
    DrawRectangleRec(w->rect, ColorFromHSV((int) hueRotationTimer % 360, 1, 1));
}