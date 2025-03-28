#ifndef SPRITE_H
#define SPRITE_H

#include <cinttypes>
#include "jaguarleap/shared/palette.h"

class Parser;
class Writer;

struct __attribute__((packed)) SpriteHeader {
    uint16_t length;
    int16_t x;
    int16_t y;
    uint16_t w;
    uint16_t h;
    uint8_t index;
    uint8_t missing;
};

class SpriteImage {
public:
    char* data;
    uint16_t length;
    uint16_t w;
    uint16_t h;

    void convertToRGBA(char* output, Palette &pal, int remap = -1);
};

class Sprite {
public:
    SpriteHeader header;
    SpriteImage image;

    Sprite(Parser &p);
    virtual ~Sprite();
    bool save(Writer &w);
};

#endif // SPRITE_H
