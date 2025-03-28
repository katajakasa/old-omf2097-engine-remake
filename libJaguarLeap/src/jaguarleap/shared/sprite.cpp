#include "jaguarleap/shared/sprite.h"
#include "jaguarleap/fileops/parser.h"
#include "jaguarleap/fileops/writer.h"
#include "jaguarleap/shared/parserexception.h"

#include <cstring>

void SpriteImage::convertToRGBA(char* output, Palette &pal, int remap) {
    Parser p(this->data, this->length);

    // Clear output
    memset(output, 0x00, w*h*4);

    // Init some vars
    int lx = 0;
    int ly = 0;
    int pos = this->length;

    while(pos > 0) {
        int val = p.uword();
        pos -= 2;

        int dlen = val / 4;
        int opcode = val % 4;
        switch(opcode) {
            case 0:
                lx = dlen;
                break;

            case 2:
                ly = dlen;
                break;

            case 3:
                pos = -1;
                break;

            case 1: {
                for(int v = 0; v < dlen; v++) {
                    int s = p.ubyte();
                    int f = ((h - 1 - ly) * w + lx) * 4;
                    if(remap > -1) {
                        output[f+0] = (uint8_t)(pal.data[(uint8_t)pal.remaps[remap][s]][0]);
                        output[f+1] = (uint8_t)(pal.data[(uint8_t)pal.remaps[remap][s]][1]);
                        output[f+2] = (uint8_t)(pal.data[(uint8_t)pal.remaps[remap][s]][2]);
                        output[f+3] = (uint8_t)0xFF;
                    } else {
                        output[f+0] = (uint8_t)(pal.data[s][0]);
                        output[f+1] = (uint8_t)(pal.data[s][1]);
                        output[f+2] = (uint8_t)(pal.data[s][2]);
                        output[f+3] = (uint8_t)0xFF;
                    }
                    pos--;
                    lx++;
                }
                lx = 0;
            } break;
        }
    }
}

Sprite::Sprite(Parser &p) {
    // Read Sprite header
    p.data((char*)(&header), sizeof(SpriteHeader));

    // Read sprite data
    if(!header.missing) {
        image.data = new char[header.length];
        p.data(image.data, header.length);
    }
    image.w = header.w;
    image.h = header.h;
    image.length = header.length;
}

Sprite::~Sprite() {
    if(!header.missing) {
        delete[] this->image.data;
    }
}

bool Sprite::save(Writer &w) {
    // Write sprite header
    w.data((char*)(&header), sizeof(SpriteHeader));

    // Write sprite data (if not defined missing)
    if(!header.missing) {
        w.data(image.data, header.length);
    }

    return true;
}
