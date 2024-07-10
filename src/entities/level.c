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

    levels[0].enabled = true;
    levels[0].walls[0] = (struct Wall) {(Rectangle) {0.328125, 0.000000, 0.015625, 0.250000}, true};
    levels[0].walls[1] = (struct Wall) {(Rectangle) {0.328125, 0.250000, 0.171875, 0.027778}, true};
    levels[0].walls[2] = (struct Wall) {(Rectangle) {0.500000, 0.722222, 0.171875, 0.027778}, true};
    levels[0].walls[3] = (struct Wall) {(Rectangle) {0.656250, 0.750000, 0.015625, 0.250000}, true};
    levels[0].walls[4] = (struct Wall) {(Rectangle) {0.656250, 0.000000, 0.015625, 0.027778}, true};
    levels[0].walls[5] = (struct Wall) {(Rectangle) {0.500000, 0.555556, 0.015625, 0.166667}, true};
    levels[0].walls[6] = (struct Wall) {(Rectangle) {0.484375, 0.277778, 0.015625, 0.166667}, true};
    levels[0].walls[7] = (struct Wall) {(Rectangle) {0.671875, 0.305556, 0.093750, 0.027778}, true};
    levels[0].walls[8] = (struct Wall) {(Rectangle) {0.828125, 0.305556, 0.171875, 0.027778}, true};
    levels[0].walls[9] = (struct Wall) {(Rectangle) {0.656250, 0.138889, 0.015625, 0.472222}, true};
    levels[0].walls[10] = (struct Wall) {(Rectangle) {0.328125, 0.388889, 0.015625, 0.472222}, true};
    levels[0].walls[11] = (struct Wall) {(Rectangle) {0.328125, 0.972222, 0.015625, 0.027778}, true};
    levels[0].walls[12] = (struct Wall) {(Rectangle) {0.343750, 0.722222, 0.046875, 0.027778}, true};
    levels[0].walls[13] = (struct Wall) {(Rectangle) {0.453125, 0.722222, 0.046875, 0.027778}, true};
    levels[0].walls[14] = (struct Wall) {(Rectangle) {0.500000, 0.250000, 0.046875, 0.027778}, true};
    levels[0].walls[15] = (struct Wall) {(Rectangle) {0.609375, 0.250000, 0.046875, 0.027778}, true};
    levels[0].walls[16] = (struct Wall) {(Rectangle) {0.500000, 0.416667, 0.015625, 0.027778}, true};
    levels[0].walls[17] = (struct Wall) {(Rectangle) {0.484375, 0.555556, 0.015625, 0.027778}, true};
    levels[0].walls[18] = (struct Wall) {(Rectangle) {0.000000, 0.666667, 0.171875, 0.027778}, true};
    levels[0].walls[19] = (struct Wall) {(Rectangle) {0.234375, 0.666667, 0.093750, 0.027778}, true};
    levels[0].walls[20] = (struct Wall) {(Rectangle) {0.078125, 0.250000, 0.171875, 0.027778}, true};
    levels[0].walls[21] = (struct Wall) {(Rectangle) {0.000000, 0.250000, 0.015625, 0.027778}, true};
    levels[0].walls[22] = (struct Wall) {(Rectangle) {0.312500, 0.250000, 0.015625, 0.027778}, true};
    levels[0].walls[23] = (struct Wall) {(Rectangle) {0.671875, 0.722222, 0.015625, 0.027778}, true};
    levels[0].walls[24] = (struct Wall) {(Rectangle) {0.750000, 0.722222, 0.171875, 0.027778}, true};
    levels[0].walls[25] = (struct Wall) {(Rectangle) {0.984375, 0.722222, 0.015625, 0.027778}, true};

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
        }

        int borderWallThickness = 3;
        levels[i].walls[maxWallCount - 4] = (struct Wall) {
                (Rectangle) {0, 0, borderWallThickness, screenHeight}, true
        };
        levels[i].walls[maxWallCount - 3] = (struct Wall) {
                (Rectangle) {0, 0, screenWidth, borderWallThickness}, true
        };
        levels[i].walls[maxWallCount - 2] = (struct Wall) {
                (Rectangle) {screenWidth - borderWallThickness, 0, borderWallThickness, screenHeight}, true
        };
        levels[i].walls[maxWallCount - 1] = (struct Wall) {
                (Rectangle) {0, screenHeight - borderWallThickness, screenWidth, borderWallThickness}, true
        };
    }
}
