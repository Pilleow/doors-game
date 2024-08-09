//
// Created by igor on 2024.08.05..
//

#include "raylib.h"
#include "raymath.h"
#include "player.h"
#include "wall.h"
#include "../constants.h"

#ifndef DOORS_GAME_PICKUPITEM_H
#define DOORS_GAME_PICKUPITEM_H

#define maximumPickupItemPositionSearchTries 30

struct PickupItem {
    PickupItemType pickupItemType;
    BooletType booletType;
    Vector2 pos;
    unsigned int pickupRadius;
    Vector2 spriteSize;
    Vector2 wallCollisionSize;
    Texture2D *textureAddr;
    int rotation;
    bool enabled;
};

void InitPickupItem(struct PickupItem *p, Texture2D *textureAddr);

bool SetRandomPickupItemPosition(struct PickupItem *p, struct Wall *walls, struct PickupItem *pickupItemArray, Location atCorner, struct Player *players);

void FixPickupItemPositionAgainstWall(struct Wall *w, Rectangle *pItemRect);

void UpdatePickupItem(struct PickupItem *pi, struct Player *players);

void DrawPickupItem(struct PickupItem *p);

void ApplyPickupItemOnPlayer(struct PickupItem *pi, struct Player *pl);

#endif //DOORS_GAME_PICKUPITEM_H
