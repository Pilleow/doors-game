//
// Created by igor on 2024.08.05..
//

#include <stdlib.h>
#include <stdio.h>
#include "pickupItem.h"
#include "../screens.h"

void InitPickupItem(struct PickupItem *p, Texture2D *textureAddr) {
    p->enabled = true;
    p->pickupRadius = 5;
    p->wallCollisionSize = (Vector2) {200, 200};
    p->rotation = 0;

    p->textureAddr = textureAddr;
    p->spriteSize = (Vector2) {26, 26};

    p->pickupItemType = BOOLETSWAP;
    do {
        p->booletType = rand() % booletTypeCount;
    } while (p->booletType == STRAIGHT);

    p->pos = (Vector2) {-1000, -1000};
}

bool SetRandomPickupItemPosition(struct PickupItem *p, struct Wall *walls, struct PickupItem *pickupItemArray,
                                 Location atCorner, struct Player *players) {
    Rectangle pItemRect;
    bool shouldRetry;
    Vector2 posDelta;

    switch (atCorner) {
        case TOPLEFT:
            posDelta.x = 0;
            posDelta.y = 0;
            break;
        case TOPRIGHT:
            posDelta.x = screenWidth / 2;
            posDelta.y = 0;
            break;
        case BOTTOMLEFT:
            posDelta.x = 0;
            posDelta.y = screenHeight / 2;
            break;
        case BOTTOMRIGHT:
            posDelta.x = screenWidth / 2;
            posDelta.y = screenHeight / 2;
            break;
    }

    for (int attempt = 0; attempt < maximumPickupItemPositionSearchTries; ++attempt) {
        pItemRect = (Rectangle) {
                (rand() % (screenWidth / 2 - (int) p->wallCollisionSize.x)) + posDelta.x,
                (rand() % (screenHeight / 2 - (int) p->wallCollisionSize.y)) + posDelta.y,
                p->wallCollisionSize.x, p->wallCollisionSize.y
        };

        for (int index = 0; index < maxWallCount; ++index) {
            if (CheckCollisionRecs(pItemRect, walls[index].rect)) {
                FixPickupItemPositionAgainstWall(&walls[index], &pItemRect);
            }
        }

        Vector2 itemCenter = (Vector2) {
                pItemRect.x + p->wallCollisionSize.x / 2,
                pItemRect.y + p->wallCollisionSize.y / 2
        };

        shouldRetry = false;
        for (int index = 0; index < maxWallCount; ++index) {
            if (CheckCollisionRecs(pItemRect, walls[index].rect)) {
                shouldRetry = true;
                break;
            }
        }
        if (shouldRetry) continue;
        for (int index = 0; index < playerCount; ++index) {
            Vector2 playerCenter = (Vector2) {
                players[index].rect.x + players[index].rect.width / 2,
                players[index].rect.y + players[index].rect.height / 2,
            };
            if (Vector2DistanceSqr(itemCenter, playerCenter) < p->wallCollisionSize.x * p->wallCollisionSize.x) {
                shouldRetry = true;
                break;
            }
        }
        if (shouldRetry) continue;
        for (int index = 0; index < maxPickupItems; ++index) {
            if (&pickupItemArray[index] == p) continue;
            if (!pickupItemArray[index].enabled) continue;
            if (Vector2DistanceSqr(pickupItemArray[index].pos, (Vector2) {pItemRect.x, pItemRect.y}) <
                16 * p->pickupRadius * p->pickupRadius) {
                shouldRetry = true;
                break;
            }
        }
        if (shouldRetry) continue;

        p->pos = (Vector2) {
                pItemRect.x, pItemRect.y
        };
        return true;
    }
    return false;
}

void FixPickupItemPositionAgainstWall(struct Wall *w, Rectangle *pItemRect) {
    // Calculation of centers of rectangles
    const Vector2 centerPlayer = {pItemRect->x + pItemRect->width / 2,
                                  pItemRect->y + pItemRect->height / 2};
    const Vector2 centerWall = {
            w->rect.x +
            w->rect.width / 2,
            w->rect.y +
            w->rect.height / 2};

    // Calculation of the distance vector between the centers of the rectangles
    Vector2 delta = (Vector2) {
            centerPlayer.x - centerWall.x,
            centerPlayer.y - centerWall.y
    };

    // Calculation of half-widths and half-heights of rectangles
    const Vector2 hsPlayer = {pItemRect->width * .5f, pItemRect->height * .5f};
    const Vector2 hsWall = {w->rect.width * .5f, w->rect.height * .5f};

    // Calculation of the minimum distance at which the two rectangles can be separated
    const float minDistX = hsPlayer.x + hsWall.x - fabsf(delta.x);
    const float minDistY = hsPlayer.y + hsWall.y - fabsf(delta.y);

    // Adjusted object position based on minimum distance
    if (minDistX < minDistY) {
        pItemRect->x += copysignf(minDistX, delta.x);
    } else {
        pItemRect->y += copysignf(minDistY, delta.y);
    }
}

void UpdatePickupItem(struct PickupItem *pi, struct Player *players) {
    pi->rotation = 30 * sinf(GetTime() * 2 + pi->booletType);
//    Vector2 itemCenter = (Vector2) {
//            pi->pos.x + pi->wallCollisionSize.x / 2,
//            pi->pos.y + pi->wallCollisionSize.y / 2
//    };
//    for (int i = 0; i < playerCount; ++i) {
//        if (!players[i].isPlaying || players[i].isDead) continue;
//        Vector2 playerCenter = (Vector2) {
//            players[i].rect.x + players[i].rect.width / 2,
//            players[i].rect.y + players[i].rect.height / 2
//        };
//        float dist = Vector2Distance(playerCenter, itemCenter);
//        if (dist < pi->wallCollisionSize.x) {
//            Vector2 delta = Vector2Subtract(playerCenter, itemCenter);
//
//        }
//    }
}

void DrawPickupItem(struct PickupItem *p) {
    Vector2 center = (Vector2) {
            p->pos.x + p->wallCollisionSize.x / 2,
            p->pos.y + p->wallCollisionSize.y / 2
    };
    DrawTexturePro(
            texGlow,
            (Rectangle) {0, 0, texGlow.width, texGlow.height},
            (Rectangle) {
                    center.x - 5 * p->spriteSize.x / 2,
                    center.y - 5 * p->spriteSize.y / 2,
                    5 * p->spriteSize.x, 5 * p->spriteSize.y
            },
            (Vector2) {0, 0},
            0,
            WHITE
    );
    DrawTexturePro(
            booletIconSprites[p->booletType],
            (Rectangle) {
                    0, 0, booletIconSprites[p->booletType].width, booletIconSprites[p->booletType].height
            },
            (Rectangle) {
                    center.x,
                    center.y,
                    2 * p->spriteSize.x, 2 * p->spriteSize.y
            },
            p->spriteSize,
            p->rotation,
            WHITE
    );
}

void ApplyPickupItemOnPlayer(struct PickupItem *pi, struct Player *pl) {
    switch (pi->pickupItemType) {
        case BOOLETSWAP:
            pl->booletType = pi->booletType;
            break;
    }
}
