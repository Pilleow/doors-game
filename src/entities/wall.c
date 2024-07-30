//
// Created by igor on 2024.07.09..
//

#include <stdio.h>
#include <stdlib.h>

#include "wall.h"
#include "../constants.h"

bool CheckWallBooletCollisionAndFixPosition(struct Wall *w, struct Boolet *e) {
    if (!CheckCollisionRecs(w->rect, e->rect)) return false;
    if (e->type == BOUNCING && e->type == INVERTED) {
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

void FixPlayerPositionAgainstWall(struct Wall *w, struct Player *p) {// Calculation of centers of rectangles
    const Vector2 center1 = {p->rect.x + p->rect.width / 2,
                             p->rect.y + p->rect.height / 2};
    const Vector2 center2 = {
            w->rect.x +
            w->rect.width / 2,
            w->rect.y +
            w->rect.height / 2};

    // Calculation of the distance vector between the centers of the rectangles
    Vector2 delta = (Vector2) {
            center1.x - center2.x,
            center1.y - center2.y
    };

    // Calculation of half-widths and half-heights of rectangles
    const Vector2 hs1 = {p->rect.width * .5f, p->rect.height * .5f};
    const Vector2 hs2 = {w->rect.width * .5f,
                         w->rect.height * .5f};

    // Calculation of the minimum distance at which the two rectangles can be separated
    const float minDistX = hs1.x + hs2.x - fabsf(delta.x);
    const float minDistY = hs1.y + hs2.y - fabsf(delta.y);

    // Adjusted object position based on minimum distance
    if (minDistX < minDistY) {
        p->rect.x += copysignf(minDistX, delta.x);
    } else {
        p->rect.y += copysignf(minDistY, delta.y);
    }
}

void DrawWall(struct Wall *w) {
    DrawRectangleRec(w->rect, ColorFromHSV((int) hueRotationTimer % 360, 1, 1));
}
