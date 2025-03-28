#pragma once

#include <string>

class Texture;
class Parser;
class Palette;

class Image {
public:
    enum ImageType {
        VGA = 0,
        SPRITE = 1
    };

    // Image data
    int w,h,depth;

    // Raw OMF data
    int rawsize;
    char* rawdata;
    ImageType imagetype;

    // Functions
    Image(int w, int h, int rsize, Parser *parser, ImageType itype);
    ~Image();

    unsigned int size() { return w*h*depth; }

    bool convert(char* data, Palette *pal);
    Texture* getTexture(Palette* pal);
    void writeTGA(Palette *pal, std::string file);

    static bool parseVga(char* output, Parser *parser, Palette *pal, int w, int h);
    static bool parseSprite(char* output, Parser *parser, Palette *palette, int w, int h, int nlen);
};
