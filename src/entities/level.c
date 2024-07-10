//
// Created by igor on 2024.07.09..
//

#include <stdio.h>
#include "level.h"
#include "../constants.h"

void LoadAllLevels(struct Level levels[]) {
    for (int i = 0; i < levelCount; ++i) {
        levels[i].enabled = false;
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

    levels[1].enabled = true;
    levels[1].walls[0] = (struct Wall) {(Rectangle) {0.531250, 0.250000, 0.234375, 0.027778}, true};
    levels[1].walls[1] = (struct Wall) {(Rectangle) {0.531250, 0.722222, 0.234375, 0.027778}, true};
    levels[1].walls[2] = (struct Wall) {(Rectangle) {0.234375, 0.722222, 0.234375, 0.027778}, true};
    levels[1].walls[3] = (struct Wall) {(Rectangle) {0.234375, 0.250000, 0.234375, 0.027778}, true};
    levels[1].walls[4] = (struct Wall) {(Rectangle) {0.000000, 0.250000, 0.171875, 0.027778}, true};
    levels[1].walls[5] = (struct Wall) {(Rectangle) {0.000000, 0.722222, 0.171875, 0.027778}, true};
    levels[1].walls[6] = (struct Wall) {(Rectangle) {0.828125, 0.722222, 0.171875, 0.027778}, true};
    levels[1].walls[7] = (struct Wall) {(Rectangle) {0.828125, 0.250000, 0.171875, 0.027778}, true};
    levels[1].walls[8] = (struct Wall) {(Rectangle) {0.640625, 0.277778, 0.015625, 0.055556}, true};
    levels[1].walls[9] = (struct Wall) {(Rectangle) {0.640625, 0.444444, 0.015625, 0.111111}, true};
    levels[1].walls[10] = (struct Wall) {(Rectangle) {0.640625, 0.666667, 0.015625, 0.055556}, true};
    levels[1].walls[11] = (struct Wall) {(Rectangle) {0.343750, 0.277778, 0.015625, 0.055556}, true};
    levels[1].walls[12] = (struct Wall) {(Rectangle) {0.343750, 0.444444, 0.015625, 0.111111}, true};
    levels[1].walls[13] = (struct Wall) {(Rectangle) {0.343750, 0.666667, 0.015625, 0.055556}, true};
    levels[1].walls[14] = (struct Wall) {(Rectangle) {0.359375, 0.444444, 0.156250, 0.027778}, true};
    levels[1].walls[15] = (struct Wall) {(Rectangle) {0.484375, 0.527778, 0.156250, 0.027778}, true};
    levels[1].walls[16] = (struct Wall) {(Rectangle) {0.328125, 0.222222, 0.015625, 0.027778}, true};
    levels[1].walls[17] = (struct Wall) {(Rectangle) {0.328125, 0.000000, 0.015625, 0.166667}, true};
    levels[1].walls[18] = (struct Wall) {(Rectangle) {0.656250, 0.000000, 0.015625, 0.166667}, true};
    levels[1].walls[19] = (struct Wall) {(Rectangle) {0.656250, 0.222222, 0.015625, 0.027778}, true};
    levels[1].walls[20] = (struct Wall) {(Rectangle) {0.328125, 0.750000, 0.015625, 0.027778}, true};
    levels[1].walls[21] = (struct Wall) {(Rectangle) {0.328125, 0.833333, 0.015625, 0.166667}, true};
    levels[1].walls[22] = (struct Wall) {(Rectangle) {0.656250, 0.833333, 0.015625, 0.166667}, true};
    levels[1].walls[23] = (struct Wall) {(Rectangle) {0.656250, 0.750000, 0.015625, 0.027778}, true};

    levels[2].enabled = true;
    levels[2].walls[0] = (struct Wall) {(Rectangle) {0.328125, 0.000000, 0.015625, 0.027778}, true};
    levels[2].walls[1] = (struct Wall) {(Rectangle) {0.328125, 0.083333, 0.015625, 0.027778}, true};
    levels[2].walls[2] = (struct Wall) {(Rectangle) {0.234375, 0.111111, 0.109375, 0.027778}, true};
    levels[2].walls[3] = (struct Wall) {(Rectangle) {0.234375, 0.138889, 0.015625, 0.027778}, true};
    levels[2].walls[4] = (struct Wall) {(Rectangle) {0.046875, 0.305556, 0.203125, 0.027778}, true};
    levels[2].walls[5] = (struct Wall) {(Rectangle) {0.000000, 0.305556, 0.015625, 0.027778}, true};
    levels[2].walls[6] = (struct Wall) {(Rectangle) {0.000000, 0.666667, 0.015625, 0.027778}, true};
    levels[2].walls[7] = (struct Wall) {(Rectangle) {0.046875, 0.666667, 0.203125, 0.027778}, true};
    levels[2].walls[8] = (struct Wall) {(Rectangle) {0.234375, 0.833333, 0.015625, 0.027778}, true};
    levels[2].walls[9] = (struct Wall) {(Rectangle) {0.234375, 0.861111, 0.109375, 0.027778}, true};
    levels[2].walls[10] = (struct Wall) {(Rectangle) {0.328125, 0.888889, 0.015625, 0.027778}, true};
    levels[2].walls[11] = (struct Wall) {(Rectangle) {0.328125, 0.972222, 0.015625, 0.027778}, true};
    levels[2].walls[12] = (struct Wall) {(Rectangle) {0.656250, 0.972222, 0.015625, 0.027778}, true};
    levels[2].walls[13] = (struct Wall) {(Rectangle) {0.656250, 0.888889, 0.015625, 0.027778}, true};
    levels[2].walls[14] = (struct Wall) {(Rectangle) {0.656250, 0.861111, 0.109375, 0.027778}, true};
    levels[2].walls[15] = (struct Wall) {(Rectangle) {0.750000, 0.833333, 0.015625, 0.027778}, true};
    levels[2].walls[16] = (struct Wall) {(Rectangle) {0.750000, 0.666667, 0.203125, 0.027778}, true};
    levels[2].walls[17] = (struct Wall) {(Rectangle) {0.984375, 0.666667, 0.015625, 0.027778}, true};
    levels[2].walls[18] = (struct Wall) {(Rectangle) {0.984375, 0.305556, 0.015625, 0.027778}, true};
    levels[2].walls[19] = (struct Wall) {(Rectangle) {0.750000, 0.305556, 0.203125, 0.027778}, true};
    levels[2].walls[20] = (struct Wall) {(Rectangle) {0.750000, 0.138889, 0.015625, 0.027778}, true};
    levels[2].walls[21] = (struct Wall) {(Rectangle) {0.656250, 0.111111, 0.109375, 0.027778}, true};
    levels[2].walls[22] = (struct Wall) {(Rectangle) {0.656250, 0.083333, 0.015625, 0.027778}, true};
    levels[2].walls[23] = (struct Wall) {(Rectangle) {0.656250, 0.000000, 0.015625, 0.027778}, true};
    levels[2].walls[24] = (struct Wall) {(Rectangle) {0.375000, 0.305556, 0.015625, 0.027778}, true};
    levels[2].walls[25] = (struct Wall) {(Rectangle) {0.375000, 0.277778, 0.250000, 0.027778}, true};
    levels[2].walls[26] = (struct Wall) {(Rectangle) {0.609375, 0.305556, 0.015625, 0.027778}, true};
    levels[2].walls[27] = (struct Wall) {(Rectangle) {0.609375, 0.666667, 0.015625, 0.027778}, true};
    levels[2].walls[28] = (struct Wall) {(Rectangle) {0.375000, 0.694444, 0.250000, 0.027778}, true};
    levels[2].walls[29] = (struct Wall) {(Rectangle) {0.375000, 0.666667, 0.015625, 0.027778}, true};
    levels[2].walls[30] = (struct Wall) {(Rectangle) {0.234375, 0.444444, 0.015625, 0.111111}, true};
    levels[2].walls[31] = (struct Wall) {(Rectangle) {0.750000, 0.444444, 0.015625, 0.111111}, true};
    levels[2].walls[32] = (struct Wall) {(Rectangle) {0.484375, 0.472222, 0.031250, 0.055556}, true};

    levels[3].enabled = true;
    levels[3].walls[0] = (struct Wall) {(Rectangle) {0.250000, 0.250000, 0.062500, 0.027778}, true};
    levels[3].walls[1] = (struct Wall) {(Rectangle) {0.250000, 0.277778, 0.015625, 0.083333}, true};
    levels[3].walls[2] = (struct Wall) {(Rectangle) {0.625000, 0.361111, 0.015625, 0.111111}, true};
    levels[3].walls[3] = (struct Wall) {(Rectangle) {0.687500, 0.250000, 0.062500, 0.027778}, true};
    levels[3].walls[4] = (struct Wall) {(Rectangle) {0.734375, 0.277778, 0.015625, 0.083333}, true};
    levels[3].walls[5] = (struct Wall) {(Rectangle) {0.546875, 0.555556, 0.015625, 0.111111}, true};
    levels[3].walls[6] = (struct Wall) {(Rectangle) {0.609375, 0.750000, 0.062500, 0.027778}, true};
    levels[3].walls[7] = (struct Wall) {(Rectangle) {0.656250, 0.666667, 0.015625, 0.083333}, true};
    levels[3].walls[8] = (struct Wall) {(Rectangle) {0.328125, 0.750000, 0.062500, 0.027778}, true};
    levels[3].walls[9] = (struct Wall) {(Rectangle) {0.328125, 0.666667, 0.015625, 0.083333}, true};
    levels[3].walls[10] = (struct Wall) {(Rectangle) {0.437500, 0.555556, 0.015625, 0.111111}, true};
    levels[3].walls[11] = (struct Wall) {(Rectangle) {0.453125, 0.222222, 0.015625, 0.083333}, true};
    levels[3].walls[12] = (struct Wall) {(Rectangle) {0.531250, 0.222222, 0.015625, 0.083333}, true};
    levels[3].walls[13] = (struct Wall) {(Rectangle) {0.468750, 0.277778, 0.062500, 0.027778}, true};
    levels[3].walls[14] = (struct Wall) {(Rectangle) {0.296875, 0.000000, 0.015625, 0.250000}, true};
    levels[3].walls[15] = (struct Wall) {(Rectangle) {0.687500, 0.000000, 0.015625, 0.250000}, true};
    levels[3].walls[16] = (struct Wall) {(Rectangle) {0.359375, 0.361111, 0.015625, 0.111111}, true};
    levels[3].walls[17] = (struct Wall) {(Rectangle) {0.437500, 0.750000, 0.125000, 0.027778}, true};
    levels[3].walls[18] = (struct Wall) {(Rectangle) {0.343750, 0.361111, 0.015625, 0.027778}, true};
    levels[3].walls[19] = (struct Wall) {(Rectangle) {0.640625, 0.361111, 0.015625, 0.027778}, true};
    levels[3].walls[20] = (struct Wall) {(Rectangle) {0.250000, 0.500000, 0.015625, 0.277778}, true};
    levels[3].walls[21] = (struct Wall) {(Rectangle) {0.734375, 0.500000, 0.015625, 0.277778}, true};
    levels[3].walls[22] = (struct Wall) {(Rectangle) {0.750000, 0.750000, 0.203125, 0.027778}, true};
    levels[3].walls[23] = (struct Wall) {(Rectangle) {0.984375, 0.750000, 0.015625, 0.027778}, true};
    levels[3].walls[24] = (struct Wall) {(Rectangle) {0.046875, 0.750000, 0.203125, 0.027778}, true};
    levels[3].walls[25] = (struct Wall) {(Rectangle) {0.000000, 0.750000, 0.015625, 0.027778}, true};
    levels[3].walls[26] = (struct Wall) {(Rectangle) {0.250000, 0.916667, 0.015625, 0.083333}, true};
    levels[3].walls[27] = (struct Wall) {(Rectangle) {0.328125, 0.916667, 0.015625, 0.083333}, true};
    levels[3].walls[28] = (struct Wall) {(Rectangle) {0.656250, 0.916667, 0.015625, 0.083333}, true};
    levels[3].walls[29] = (struct Wall) {(Rectangle) {0.734375, 0.972222, 0.015625, 0.027778}, true};
    levels[3].walls[30] = (struct Wall) {(Rectangle) {0.828125, 0.250000, 0.171875, 0.027778}, true};
    levels[3].walls[31] = (struct Wall) {(Rectangle) {0.000000, 0.250000, 0.171875, 0.027778}, true};
    levels[3].walls[32] = (struct Wall) {(Rectangle) {0.437500, 0.222222, 0.015625, 0.027778}, true};
    levels[3].walls[33] = (struct Wall) {(Rectangle) {0.546875, 0.222222, 0.015625, 0.027778}, true};

    levels[4].enabled = true;
    levels[4].walls[0] = (struct Wall) {(Rectangle) {0.750000, 0.000000, 0.015625, 0.027778}, true};
    levels[4].walls[1] = (struct Wall) {(Rectangle) {0.750000, 0.138889, 0.015625, 0.083333}, true};
    levels[4].walls[2] = (struct Wall) {(Rectangle) {0.750000, 0.222222, 0.031250, 0.027778}, true};
    levels[4].walls[3] = (struct Wall) {(Rectangle) {0.265625, 0.222222, 0.015625, 0.027778}, true};
    levels[4].walls[4] = (struct Wall) {(Rectangle) {0.296875, 0.222222, 0.015625, 0.027778}, true};
    levels[4].walls[5] = (struct Wall) {(Rectangle) {0.328125, 0.222222, 0.015625, 0.027778}, true};
    levels[4].walls[6] = (struct Wall) {(Rectangle) {0.359375, 0.222222, 0.015625, 0.027778}, true};
    levels[4].walls[7] = (struct Wall) {(Rectangle) {0.390625, 0.222222, 0.015625, 0.083333}, true};
    levels[4].walls[8] = (struct Wall) {(Rectangle) {0.390625, 0.416667, 0.015625, 0.361111}, true};
    levels[4].walls[9] = (struct Wall) {(Rectangle) {0.593750, 0.222222, 0.015625, 0.361111}, true};
    levels[4].walls[10] = (struct Wall) {(Rectangle) {0.593750, 0.694444, 0.015625, 0.083333}, true};
    levels[4].walls[11] = (struct Wall) {(Rectangle) {0.625000, 0.222222, 0.015625, 0.027778}, true};
    levels[4].walls[12] = (struct Wall) {(Rectangle) {0.656250, 0.222222, 0.015625, 0.027778}, true};
    levels[4].walls[13] = (struct Wall) {(Rectangle) {0.687500, 0.222222, 0.015625, 0.027778}, true};
    levels[4].walls[14] = (struct Wall) {(Rectangle) {0.718750, 0.222222, 0.015625, 0.027778}, true};
    levels[4].walls[15] = (struct Wall) {(Rectangle) {0.234375, 0.750000, 0.015625, 0.111111}, true};
    levels[4].walls[16] = (struct Wall) {(Rectangle) {0.234375, 0.972222, 0.015625, 0.027778}, true};
    levels[4].walls[17] = (struct Wall) {(Rectangle) {0.218750, 0.750000, 0.015625, 0.027778}, true};
    levels[4].walls[18] = (struct Wall) {(Rectangle) {0.000000, 0.750000, 0.156250, 0.027778}, true};
    levels[4].walls[19] = (struct Wall) {(Rectangle) {0.265625, 0.750000, 0.015625, 0.027778}, true};
    levels[4].walls[20] = (struct Wall) {(Rectangle) {0.296875, 0.750000, 0.015625, 0.027778}, true};
    levels[4].walls[21] = (struct Wall) {(Rectangle) {0.328125, 0.750000, 0.015625, 0.027778}, true};
    levels[4].walls[22] = (struct Wall) {(Rectangle) {0.359375, 0.750000, 0.015625, 0.027778}, true};
    levels[4].walls[23] = (struct Wall) {(Rectangle) {0.625000, 0.750000, 0.015625, 0.027778}, true};
    levels[4].walls[24] = (struct Wall) {(Rectangle) {0.656250, 0.750000, 0.015625, 0.027778}, true};
    levels[4].walls[25] = (struct Wall) {(Rectangle) {0.687500, 0.750000, 0.015625, 0.027778}, true};
    levels[4].walls[26] = (struct Wall) {(Rectangle) {0.718750, 0.750000, 0.015625, 0.027778}, true};
    levels[4].walls[27] = (struct Wall) {(Rectangle) {0.093750, 0.222222, 0.156250, 0.027778}, true};
    levels[4].walls[28] = (struct Wall) {(Rectangle) {0.000000, 0.222222, 0.031250, 0.027778}, true};
    levels[4].walls[29] = (struct Wall) {(Rectangle) {0.843750, 0.222222, 0.156250, 0.027778}, true};
    levels[4].walls[30] = (struct Wall) {(Rectangle) {0.968750, 0.750000, 0.031250, 0.027778}, true};
    levels[4].walls[31] = (struct Wall) {(Rectangle) {0.750000, 0.750000, 0.156250, 0.027778}, true};
    levels[4].walls[32] = (struct Wall) {(Rectangle) {0.750000, 0.777778, 0.015625, 0.027778}, true};
    levels[4].walls[33] = (struct Wall) {(Rectangle) {0.750000, 0.916667, 0.015625, 0.083333}, true};
    levels[4].walls[34] = (struct Wall) {(Rectangle) {0.234375, 0.000000, 0.015625, 0.083333}, true};
    levels[4].walls[35] = (struct Wall) {(Rectangle) {0.234375, 0.194444, 0.015625, 0.027778}, true};
    levels[4].walls[36] = (struct Wall) {(Rectangle) {0.406250, 0.222222, 0.015625, 0.027778}, true};
    levels[4].walls[37] = (struct Wall) {(Rectangle) {0.578125, 0.222222, 0.015625, 0.027778}, true};
    levels[4].walls[38] = (struct Wall) {(Rectangle) {0.406250, 0.750000, 0.015625, 0.027778}, true};
    levels[4].walls[39] = (struct Wall) {(Rectangle) {0.578125, 0.750000, 0.015625, 0.027778}, true};
    levels[4].walls[40] = (struct Wall) {(Rectangle) {0.750000, 0.611111, 0.125000, 0.027778}, true};
    levels[4].walls[41] = (struct Wall) {(Rectangle) {0.750000, 0.361111, 0.125000, 0.027778}, true};
    levels[4].walls[42] = (struct Wall) {(Rectangle) {0.125000, 0.611111, 0.125000, 0.027778}, true};
    levels[4].walls[43] = (struct Wall) {(Rectangle) {0.125000, 0.361111, 0.125000, 0.027778}, true};
    levels[4].walls[44] = (struct Wall) {(Rectangle) {0.375000, 0.416667, 0.015625, 0.027778}, true};
    levels[4].walls[45] = (struct Wall) {(Rectangle) {0.609375, 0.555556, 0.015625, 0.027778}, true};
    levels[4].walls[46] = (struct Wall) {(Rectangle) {0.484375, 0.416667, 0.031250, 0.166667}, true};

    levels[5].enabled = true;
    levels[5].walls[0] = (struct Wall) {(Rectangle) {0.937500, 0.250000, 0.062500, 0.027778}, true};
    levels[5].walls[1] = (struct Wall) {(Rectangle) {0.937500, 0.722222, 0.062500, 0.027778}, true};
    levels[5].walls[2] = (struct Wall) {(Rectangle) {0.000000, 0.250000, 0.062500, 0.027778}, true};
    levels[5].walls[3] = (struct Wall) {(Rectangle) {0.000000, 0.722222, 0.062500, 0.027778}, true};
    levels[5].walls[4] = (struct Wall) {(Rectangle) {0.234375, 0.222222, 0.015625, 0.027778}, true};
    levels[5].walls[5] = (struct Wall) {(Rectangle) {0.234375, 0.000000, 0.015625, 0.111111}, true};
    levels[5].walls[6] = (struct Wall) {(Rectangle) {0.750000, 0.000000, 0.015625, 0.111111}, true};
    levels[5].walls[7] = (struct Wall) {(Rectangle) {0.750000, 0.222222, 0.015625, 0.027778}, true};
    levels[5].walls[8] = (struct Wall) {(Rectangle) {0.750000, 0.750000, 0.015625, 0.027778}, true};
    levels[5].walls[9] = (struct Wall) {(Rectangle) {0.750000, 0.888889, 0.015625, 0.111111}, true};
    levels[5].walls[10] = (struct Wall) {(Rectangle) {0.234375, 0.750000, 0.015625, 0.027778}, true};
    levels[5].walls[11] = (struct Wall) {(Rectangle) {0.234375, 0.888889, 0.015625, 0.111111}, true};
    levels[5].walls[12] = (struct Wall) {(Rectangle) {0.359375, 0.222222, 0.031250, 0.055556}, true};
    levels[5].walls[13] = (struct Wall) {(Rectangle) {0.125000, 0.722222, 0.125000, 0.027778}, true};
    levels[5].walls[14] = (struct Wall) {(Rectangle) {0.125000, 0.250000, 0.125000, 0.027778}, true};
    levels[5].walls[15] = (struct Wall) {(Rectangle) {0.750000, 0.250000, 0.125000, 0.027778}, true};
    levels[5].walls[16] = (struct Wall) {(Rectangle) {0.750000, 0.722222, 0.125000, 0.027778}, true};
    levels[5].walls[17] = (struct Wall) {(Rectangle) {0.609375, 0.222222, 0.031250, 0.055556}, true};
    levels[5].walls[18] = (struct Wall) {(Rectangle) {0.359375, 0.722222, 0.031250, 0.055556}, true};
    levels[5].walls[19] = (struct Wall) {(Rectangle) {0.609375, 0.722222, 0.031250, 0.055556}, true};
    levels[5].walls[20] = (struct Wall) {(Rectangle) {0.750000, 0.277778, 0.015625, 0.138889}, true};
    levels[5].walls[21] = (struct Wall) {(Rectangle) {0.750000, 0.583333, 0.015625, 0.138889}, true};
    levels[5].walls[22] = (struct Wall) {(Rectangle) {0.234375, 0.277778, 0.015625, 0.138889}, true};
    levels[5].walls[23] = (struct Wall) {(Rectangle) {0.234375, 0.583333, 0.015625, 0.138889}, true};
    levels[5].walls[24] = (struct Wall) {(Rectangle) {0.468750, 0.361111, 0.015625, 0.083333}, true};
    levels[5].walls[25] = (struct Wall) {(Rectangle) {0.515625, 0.361111, 0.093750, 0.027778}, true};
    levels[5].walls[26] = (struct Wall) {(Rectangle) {0.390625, 0.444444, 0.093750, 0.027778}, true};
    levels[5].walls[27] = (struct Wall) {(Rectangle) {0.515625, 0.527778, 0.093750, 0.027778}, true};
    levels[5].walls[28] = (struct Wall) {(Rectangle) {0.515625, 0.555556, 0.015625, 0.083333}, true};
    levels[5].walls[29] = (struct Wall) {(Rectangle) {0.390625, 0.611111, 0.093750, 0.027778}, true};
    levels[5].walls[30] = (struct Wall) {(Rectangle) {0.250000, 0.250000, 0.109375, 0.027778}, true};
    levels[5].walls[31] = (struct Wall) {(Rectangle) {0.640625, 0.722222, 0.109375, 0.027778}, true};

    levels[6].enabled = true;
    levels[6].walls[0] = (struct Wall) {(Rectangle) {0.328125, 0.111111, 0.015625, 0.055556}, true};
    levels[6].walls[1] = (struct Wall) {(Rectangle) {0.218750, 0.083333, 0.125000, 0.027778}, true};
    levels[6].walls[2] = (struct Wall) {(Rectangle) {0.234375, 0.277778, 0.109375, 0.027778}, true};
    levels[6].walls[3] = (struct Wall) {(Rectangle) {0.218750, 0.222222, 0.015625, 0.083333}, true};
    levels[6].walls[4] = (struct Wall) {(Rectangle) {0.656250, 0.083333, 0.015625, 0.083333}, true};
    levels[6].walls[5] = (struct Wall) {(Rectangle) {0.671875, 0.083333, 0.109375, 0.027778}, true};
    levels[6].walls[6] = (struct Wall) {(Rectangle) {0.656250, 0.277778, 0.125000, 0.027778}, true};
    levels[6].walls[7] = (struct Wall) {(Rectangle) {0.765625, 0.222222, 0.015625, 0.055556}, true};
    levels[6].walls[8] = (struct Wall) {(Rectangle) {0.421875, 0.388889, 0.015625, 0.222222}, true};
    levels[6].walls[9] = (struct Wall) {(Rectangle) {0.562500, 0.388889, 0.015625, 0.222222}, true};
    levels[6].walls[10] = (struct Wall) {(Rectangle) {0.421875, 0.222222, 0.156250, 0.027778}, true};
    levels[6].walls[11] = (struct Wall) {(Rectangle) {0.421875, 0.750000, 0.156250, 0.027778}, true};
    levels[6].walls[12] = (struct Wall) {(Rectangle) {0.656250, 0.694444, 0.125000, 0.027778}, true};
    levels[6].walls[13] = (struct Wall) {(Rectangle) {0.765625, 0.722222, 0.015625, 0.055556}, true};
    levels[6].walls[14] = (struct Wall) {(Rectangle) {0.656250, 0.888889, 0.125000, 0.027778}, true};
    levels[6].walls[15] = (struct Wall) {(Rectangle) {0.656250, 0.833333, 0.015625, 0.055556}, true};
    levels[6].walls[16] = (struct Wall) {(Rectangle) {0.218750, 0.694444, 0.125000, 0.027778}, true};
    levels[6].walls[17] = (struct Wall) {(Rectangle) {0.218750, 0.722222, 0.015625, 0.055556}, true};
    levels[6].walls[18] = (struct Wall) {(Rectangle) {0.218750, 0.888889, 0.125000, 0.027778}, true};
    levels[6].walls[19] = (struct Wall) {(Rectangle) {0.328125, 0.833333, 0.015625, 0.055556}, true};
    levels[6].walls[20] = (struct Wall) {(Rectangle) {0.046875, 0.277778, 0.125000, 0.027778}, true};
    levels[6].walls[21] = (struct Wall) {(Rectangle) {0.046875, 0.694444, 0.125000, 0.027778}, true};
    levels[6].walls[22] = (struct Wall) {(Rectangle) {0.328125, 0.666667, 0.015625, 0.027778}, true};
    levels[6].walls[23] = (struct Wall) {(Rectangle) {0.328125, 0.305556, 0.015625, 0.027778}, true};
    levels[6].walls[24] = (struct Wall) {(Rectangle) {0.218750, 0.055556, 0.015625, 0.027778}, true};
    levels[6].walls[25] = (struct Wall) {(Rectangle) {0.765625, 0.055556, 0.015625, 0.027778}, true};
    levels[6].walls[26] = (struct Wall) {(Rectangle) {0.656250, 0.305556, 0.015625, 0.027778}, true};
    levels[6].walls[27] = (struct Wall) {(Rectangle) {0.656250, 0.666667, 0.015625, 0.027778}, true};
    levels[6].walls[28] = (struct Wall) {(Rectangle) {0.765625, 0.916667, 0.015625, 0.027778}, true};
    levels[6].walls[29] = (struct Wall) {(Rectangle) {0.562500, 0.777778, 0.015625, 0.027778}, true};
    levels[6].walls[30] = (struct Wall) {(Rectangle) {0.421875, 0.777778, 0.015625, 0.027778}, true};
    levels[6].walls[31] = (struct Wall) {(Rectangle) {0.421875, 0.194444, 0.015625, 0.027778}, true};
    levels[6].walls[32] = (struct Wall) {(Rectangle) {0.562500, 0.194444, 0.015625, 0.027778}, true};
    levels[6].walls[33] = (struct Wall) {(Rectangle) {0.828125, 0.277778, 0.125000, 0.027778}, true};
    levels[6].walls[34] = (struct Wall) {(Rectangle) {0.828125, 0.694444, 0.125000, 0.027778}, true};
    levels[6].walls[35] = (struct Wall) {(Rectangle) {0.578125, 0.388889, 0.015625, 0.027778}, true};
    levels[6].walls[36] = (struct Wall) {(Rectangle) {0.546875, 0.583333, 0.015625, 0.027778}, true};
    levels[6].walls[37] = (struct Wall) {(Rectangle) {0.437500, 0.388889, 0.015625, 0.027778}, true};
    levels[6].walls[38] = (struct Wall) {(Rectangle) {0.406250, 0.583333, 0.015625, 0.027778}, true};

    levels[7].enabled = true;
    levels[7].walls[0] = (struct Wall) {(Rectangle) {0.250000, 0.111111, 0.093750, 0.027778}, true};
    levels[7].walls[1] = (struct Wall) {(Rectangle) {0.250000, 0.138889, 0.015625, 0.138889}, true};
    levels[7].walls[2] = (struct Wall) {(Rectangle) {0.125000, 0.250000, 0.125000, 0.027778}, true};
    levels[7].walls[3] = (struct Wall) {(Rectangle) {0.125000, 0.277778, 0.015625, 0.055556}, true};
    levels[7].walls[4] = (struct Wall) {(Rectangle) {0.125000, 0.722222, 0.140625, 0.027778}, true};
    levels[7].walls[5] = (struct Wall) {(Rectangle) {0.250000, 0.750000, 0.015625, 0.138889}, true};
    levels[7].walls[6] = (struct Wall) {(Rectangle) {0.265625, 0.861111, 0.078125, 0.027778}, true};
    levels[7].walls[7] = (struct Wall) {(Rectangle) {0.125000, 0.666667, 0.015625, 0.055556}, true};
    levels[7].walls[8] = (struct Wall) {(Rectangle) {0.312500, 0.361111, 0.015625, 0.277778}, true};
    levels[7].walls[9] = (struct Wall) {(Rectangle) {0.671875, 0.361111, 0.015625, 0.277778}, true};
    levels[7].walls[10] = (struct Wall) {(Rectangle) {0.656250, 0.111111, 0.093750, 0.027778}, true};
    levels[7].walls[11] = (struct Wall) {(Rectangle) {0.734375, 0.138889, 0.015625, 0.138889}, true};
    levels[7].walls[12] = (struct Wall) {(Rectangle) {0.750000, 0.250000, 0.125000, 0.027778}, true};
    levels[7].walls[13] = (struct Wall) {(Rectangle) {0.859375, 0.277778, 0.015625, 0.055556}, true};
    levels[7].walls[14] = (struct Wall) {(Rectangle) {0.859375, 0.666667, 0.015625, 0.055556}, true};
    levels[7].walls[15] = (struct Wall) {(Rectangle) {0.734375, 0.722222, 0.140625, 0.027778}, true};
    levels[7].walls[16] = (struct Wall) {(Rectangle) {0.734375, 0.750000, 0.015625, 0.138889}, true};
    levels[7].walls[17] = (struct Wall) {(Rectangle) {0.656250, 0.861111, 0.078125, 0.027778}, true};
    levels[7].walls[18] = (struct Wall) {(Rectangle) {0.375000, 0.250000, 0.250000, 0.027778}, true};
    levels[7].walls[19] = (struct Wall) {(Rectangle) {0.375000, 0.722222, 0.250000, 0.027778}, true};
    levels[7].walls[20] = (struct Wall) {(Rectangle) {0.468750, 0.472222, 0.062500, 0.055556}, true};
    levels[7].walls[21] = (struct Wall) {(Rectangle) {0.109375, 0.305556, 0.015625, 0.027778}, true};
    levels[7].walls[22] = (struct Wall) {(Rectangle) {0.109375, 0.666667, 0.015625, 0.027778}, true};
    levels[7].walls[23] = (struct Wall) {(Rectangle) {0.328125, 0.888889, 0.015625, 0.027778}, true};
    levels[7].walls[24] = (struct Wall) {(Rectangle) {0.656250, 0.888889, 0.015625, 0.027778}, true};
    levels[7].walls[25] = (struct Wall) {(Rectangle) {0.875000, 0.666667, 0.015625, 0.027778}, true};
    levels[7].walls[26] = (struct Wall) {(Rectangle) {0.875000, 0.305556, 0.015625, 0.027778}, true};
    levels[7].walls[27] = (struct Wall) {(Rectangle) {0.656250, 0.083333, 0.015625, 0.027778}, true};
    levels[7].walls[28] = (struct Wall) {(Rectangle) {0.328125, 0.083333, 0.015625, 0.027778}, true};
    levels[7].walls[29] = (struct Wall) {(Rectangle) {0.328125, 0.361111, 0.015625, 0.027778}, true};
    levels[7].walls[30] = (struct Wall) {(Rectangle) {0.375000, 0.277778, 0.015625, 0.027778}, true};
    levels[7].walls[31] = (struct Wall) {(Rectangle) {0.328125, 0.611111, 0.015625, 0.027778}, true};
    levels[7].walls[32] = (struct Wall) {(Rectangle) {0.375000, 0.694444, 0.015625, 0.027778}, true};
    levels[7].walls[33] = (struct Wall) {(Rectangle) {0.609375, 0.694444, 0.015625, 0.027778}, true};
    levels[7].walls[34] = (struct Wall) {(Rectangle) {0.656250, 0.611111, 0.015625, 0.027778}, true};
    levels[7].walls[35] = (struct Wall) {(Rectangle) {0.656250, 0.361111, 0.015625, 0.027778}, true};
    levels[7].walls[36] = (struct Wall) {(Rectangle) {0.609375, 0.277778, 0.015625, 0.027778}, true};
    levels[7].walls[37] = (struct Wall) {(Rectangle) {0.968750, 0.666667, 0.031250, 0.027778}, true};
    levels[7].walls[38] = (struct Wall) {(Rectangle) {0.968750, 0.305556, 0.031250, 0.027778}, true};
    levels[7].walls[39] = (struct Wall) {(Rectangle) {0.656250, 0.000000, 0.015625, 0.027778}, true};
    levels[7].walls[40] = (struct Wall) {(Rectangle) {0.328125, 0.000000, 0.015625, 0.027778}, true};
    levels[7].walls[41] = (struct Wall) {(Rectangle) {0.000000, 0.305556, 0.031250, 0.027778}, true};
    levels[7].walls[42] = (struct Wall) {(Rectangle) {0.000000, 0.666667, 0.031250, 0.027778}, true};
    levels[7].walls[43] = (struct Wall) {(Rectangle) {0.328125, 0.972222, 0.015625, 0.027778}, true};
    levels[7].walls[44] = (struct Wall) {(Rectangle) {0.656250, 0.972222, 0.015625, 0.027778}, true};

    levels[8].enabled = true;
    levels[8].walls[0] = (struct Wall) {(Rectangle) {0.421875, 0.305556, 0.156250, 0.027778}, true};
    levels[8].walls[1] = (struct Wall) {(Rectangle) {0.421875, 0.666667, 0.156250, 0.027778}, true};
    levels[8].walls[2] = (struct Wall) {(Rectangle) {0.328125, 0.000000, 0.015625, 0.166667}, true};
    levels[8].walls[3] = (struct Wall) {(Rectangle) {0.656250, 0.000000, 0.015625, 0.166667}, true};
    levels[8].walls[4] = (struct Wall) {(Rectangle) {0.656250, 0.833333, 0.015625, 0.166667}, true};
    levels[8].walls[5] = (struct Wall) {(Rectangle) {0.328125, 0.833333, 0.015625, 0.166667}, true};
    levels[8].walls[6] = (struct Wall) {(Rectangle) {0.234375, 0.277778, 0.015625, 0.027778}, true};
    levels[8].walls[7] = (struct Wall) {(Rectangle) {0.234375, 0.694444, 0.015625, 0.027778}, true};
    levels[8].walls[8] = (struct Wall) {(Rectangle) {0.312500, 0.138889, 0.015625, 0.027778}, true};
    levels[8].walls[9] = (struct Wall) {(Rectangle) {0.671875, 0.138889, 0.015625, 0.027778}, true};
    levels[8].walls[10] = (struct Wall) {(Rectangle) {0.750000, 0.277778, 0.015625, 0.027778}, true};
    levels[8].walls[11] = (struct Wall) {(Rectangle) {0.671875, 0.833333, 0.015625, 0.027778}, true};
    levels[8].walls[12] = (struct Wall) {(Rectangle) {0.312500, 0.833333, 0.015625, 0.027778}, true};
    levels[8].walls[13] = (struct Wall) {(Rectangle) {0.328125, 0.305556, 0.015625, 0.138889}, true};
    levels[8].walls[14] = (struct Wall) {(Rectangle) {0.328125, 0.555556, 0.015625, 0.138889}, true};
    levels[8].walls[15] = (struct Wall) {(Rectangle) {0.343750, 0.416667, 0.015625, 0.166667}, true};
    levels[8].walls[16] = (struct Wall) {(Rectangle) {0.656250, 0.305556, 0.015625, 0.138889}, true};
    levels[8].walls[17] = (struct Wall) {(Rectangle) {0.640625, 0.416667, 0.015625, 0.166667}, true};
    levels[8].walls[18] = (struct Wall) {(Rectangle) {0.656250, 0.555556, 0.015625, 0.138889}, true};
    levels[8].walls[19] = (struct Wall) {(Rectangle) {0.156250, 0.305556, 0.093750, 0.027778}, true};
    levels[8].walls[20] = (struct Wall) {(Rectangle) {0.078125, 0.277778, 0.093750, 0.027778}, true};
    levels[8].walls[21] = (struct Wall) {(Rectangle) {0.000000, 0.277778, 0.015625, 0.027778}, true};
    levels[8].walls[22] = (struct Wall) {(Rectangle) {0.000000, 0.694444, 0.015625, 0.027778}, true};
    levels[8].walls[23] = (struct Wall) {(Rectangle) {0.156250, 0.666667, 0.093750, 0.027778}, true};
    levels[8].walls[24] = (struct Wall) {(Rectangle) {0.078125, 0.694444, 0.093750, 0.027778}, true};
    levels[8].walls[25] = (struct Wall) {(Rectangle) {0.750000, 0.305556, 0.093750, 0.027778}, true};
    levels[8].walls[26] = (struct Wall) {(Rectangle) {0.828125, 0.277778, 0.093750, 0.027778}, true};
    levels[8].walls[27] = (struct Wall) {(Rectangle) {0.750000, 0.722222, 0.015625, 0.027778}, true};
    levels[8].walls[28] = (struct Wall) {(Rectangle) {0.750000, 0.694444, 0.093750, 0.027778}, true};
    levels[8].walls[29] = (struct Wall) {(Rectangle) {0.828125, 0.722222, 0.093750, 0.027778}, true};
    levels[8].walls[30] = (struct Wall) {(Rectangle) {0.984375, 0.722222, 0.015625, 0.027778}, true};
    levels[8].walls[31] = (struct Wall) {(Rectangle) {0.984375, 0.277778, 0.015625, 0.027778}, true};
    levels[8].walls[32] = (struct Wall) {(Rectangle) {0.750000, 0.138889, 0.015625, 0.027778}, true};
    levels[8].walls[33] = (struct Wall) {(Rectangle) {0.750000, 0.833333, 0.015625, 0.027778}, true};
    levels[8].walls[34] = (struct Wall) {(Rectangle) {0.234375, 0.833333, 0.015625, 0.027778}, true};
    levels[8].walls[35] = (struct Wall) {(Rectangle) {0.234375, 0.138889, 0.015625, 0.027778}, true};

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
