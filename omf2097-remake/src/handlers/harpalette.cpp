#include "harpalette.h"

HarPalette::HarPalette() {
    // Default palette for HAR's

    for(int i = 0; i < 16; i++) {
        palette[i][0] = i*4;
        palette[i][1] = 0;
        palette[i][2] = 0;
        palette[i][3] = 0xFF;
    }
    for(int i = 0; i < 16; i++) {
        palette[i+16][0] = 0;
        palette[i+16][1] = (i*4);
        palette[i+16][2] = 0;
        palette[i+16][3] = 0xFF;
    }
    for(int i = 0; i < 16; i++) {
        palette[i+32][0] = i;
        palette[i+32][1] = i;
        palette[i+32][2] = i;
        palette[i+32][3] = 0xFF;
    }
    palette[0][0] = 0xFF;
    palette[0][1] = 0xFF;
    palette[0][2] = 0xFF;
    palette[0][3] = 0xFF;
}
