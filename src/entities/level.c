//
// Created by igor on 2024.07.09..
//

#include <stdio.h>
#include "level.h"
#include "../constants.h"

void LoadAllLevels(struct Level levels[]) {
    for (int i = 0; i < levelCount; ++i) {
        levels[i].enabled = true;
        for (int j = 0; j < maxWallCount; ++j) levels[i].walls[j].enabled = false;
    }

    // -----------------------------------------------------------------------------------------------------------------
    // IMPORTANT NOTE: Make sure to use position and size values in [0, 1] range as they get scaled at the end
    // -----------------------------------------------------------------------------------------------------------------

    // level 0 ---------------------------------------------------------------------------------------------------------
    levels[0].enabled = true;
    levels[0].walls[1] = (struct Wall) {
            (Rectangle) {0.49, 0.3, 0.02, 0.4}, true
    };
    levels[0].walls[2] = (struct Wall) {
            (Rectangle) {0.1, 0.3, 0.2, 0.02}, true
    };
    levels[0].walls[3] = (struct Wall) {
            (Rectangle) {0.7, 0.3, 0.2, 0.02}, true
    };
    levels[0].walls[4] = (struct Wall) {
            (Rectangle) {0.1, 0.68, 0.2, 0.02}, true
    };
    levels[0].walls[5] = (struct Wall) {
            (Rectangle) {0.7, 0.68, 0.2, 0.02}, true
    };

    // level 1 ---------------------------------------------------------------------------------------------------------
    levels[1].enabled = true;
    levels[1].walls[1] = (struct Wall) {
            (Rectangle) {0.1, 0.2, 0.1, 0.17}, true
    };
    levels[1].walls[2] = (struct Wall) {
            (Rectangle) {0.8, 0.63, 0.1, 0.17}, true
    };
    levels[1].walls[3] = (struct Wall) {
            (Rectangle) {0.7, 0.05, 0.1, 0.17}, true
    };
    levels[1].walls[4] = (struct Wall) {
            (Rectangle) {0.2, 0.78, 0.1, 0.17}, true
    };
    levels[1].walls[5] = (struct Wall) {
            (Rectangle) {0.45, 0.415, 0.1, 0.17}, true
    };

    // level 2 ---------------------------------------------------------------------------------------------------------
    levels[2].enabled = true;
    levels[2].walls[1] = (struct Wall) {
            (Rectangle) {0.2, 0.25, 0.24, 0.02}, true
    };
    levels[2].walls[2] = (struct Wall) {
            (Rectangle) {0.2, 0.73, 0.24, 0.02}, true
    };
    levels[2].walls[3] = (struct Wall) {
            (Rectangle) {1 - 0.2 - 0.24, 0.25, 0.24, 0.02}, true
    };
    levels[2].walls[4] = (struct Wall) {
            (Rectangle) {1 - 0.2 - 0.24, 0.73, 0.24, 0.02}, true
    };
    levels[2].walls[5] = (struct Wall) {
            (Rectangle) {1 - 0.2, 0.25, 0.012, 0.15}, true
    };
    levels[2].walls[6] = (struct Wall) {
            (Rectangle) {1 - 0.2, 0.73 - 0.15 + 0.02, 0.012, 0.15}, true
    };
    levels[2].walls[7] = (struct Wall) {
            (Rectangle) {0.2 - 0.012, 0.25, 0.012, 0.15}, true
    };
    levels[2].walls[8] = (struct Wall) {
            (Rectangle) {0.2 - 0.012, 0.73 - 0.15 + 0.02, 0.012, 0.15}, true
    };
    levels[2].walls[9] = (struct Wall) {
            (Rectangle) {0.425, 0.5 - 0.01, 0.15, 0.02}, true
    };

    // position and size scaling here ----------------------------------------------------------------------------------
    for (int i = 0; i < levelCount; ++i) {
        if (!levels[i].enabled) continue;
        for (int j = 0; j < playerCount; ++j) {
            levels[i].spawnLocations[j].x *= screenWidth;
            levels[i].spawnLocations[j].y *= screenHeight;
        }
        for (int j = 0; j < maxWallCount; ++j) {
            if (!levels[i].walls[j].enabled) continue;
            levels[i].walls[j].rect.x *= screenWidth;
            levels[i].walls[j].rect.y *= screenHeight;
            levels[i].walls[j].rect.width *= screenWidth;
            levels[i].walls[j].rect.height *= screenHeight;
            printf("x: %f, y: %f, w: %f, h: %f\n", levels[i].walls[j].rect.x, levels[i].walls[j].rect.y,
                   levels[i].walls[j].rect.width, levels[i].walls[j].rect.height);
        }
    }
}
