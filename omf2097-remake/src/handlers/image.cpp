#include "image.h"

#include "video/tgawriter.h"
#include "video/texture.h"
#include "tools/parser.h"
#include "palette.h"
#include <cstring>

using namespace std;

Image::Image(int w, int h, int rsize, Parser *parser, ImageType itype) {
    // Uncompressed image size
    this->w = w;
    this->h = h;
    this->depth = 4;

    // Raw OMG image data
    this->imagetype = itype;
    this->rawsize = rsize;
    this->rawdata = new char[rsize];
    parser->data(this->rawdata, rsize);
}

Image::~Image() {
    if(rawsize > 0)
        delete[] rawdata;
}

bool Image::convert(char* data, Palette *palette) {
    Parser parser(this->rawdata, this->rawsize);
    if(this->imagetype == SPRITE) {
        parseSprite(data, &parser, palette, this->w, this->h, this->rawsize);
    } else {
        parseVga(data, &parser, palette, this->w, this->h);
    }
    return true;
}

Texture* Image::getTexture(Palette* pal) {
    char data[this->size()];
    this->convert(data, pal);
    return new Texture(this->w, this->h, this->depth, data);
}

void Image::writeTGA(Palette *pal, std::string file) {
    if(this->size() <= 0) {
        return;
    }

    char data[this->size()];
    if(!this->convert(data, pal)) {
        return;
    }

    TGAWriter writer(file, this->w, this->h);
    for(int i = 0; i < this->w*this->h*4; i=i+4) {
        writer.write(data[i+0],data[i+1],data[i+2],data[i+3]);
    }
    writer.finish();
}

bool Image::parseVga(char* output, Parser *parser, Palette *pal, int w, int h) {
    if(!parser->isok()) {
        return false;
    }

    memset(output, 0xFF, w*h*4);
    int pos = 0;
    for(int y = h-1; y >= 0; y--) {
        for(int x = 0; x < w; x++) {
            uint8_t b = parser->byte();
            pos = ((y * w) + x)*4;
            output[pos+0] = pal->palette[b][0];
            output[pos+1] = pal->palette[b][1];
            output[pos+2] = pal->palette[b][2];
            output[pos+3] = 0xFF;
        }
    }

    return true;
}

bool Image::parseSprite(char* output, Parser *parser, Palette *palette, int w, int h, int nlen) {
    if(!parser->isok()) {
        return false;
    }

    int lx = 0;
    int ly = 0;
    memset(output, 0x00, w*h*4);

    int pos = nlen;
    while(pos > 0) {
        int val = parser->uword();
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
                    int s = parser->ubyte();
                    int f = ((h-1-ly)*w + lx)*4;
                    output[f+0] = (uint8_t)(palette->palette[s][0]);
                    output[f+1] = (uint8_t)(palette->palette[s][1]);
                    output[f+2] = (uint8_t)(palette->palette[s][2]);
                    output[f+3] = 0xFF;
                    pos--;
                    lx++;
                }
                lx = 0;
            } break;
        }
    }
    return true;
}

