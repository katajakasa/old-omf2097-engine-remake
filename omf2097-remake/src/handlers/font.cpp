#include "handlers/font.h"
#include "tools/parser.h"
#include "tools/exception.h"
#include "tools/log.h"
#include "video/color.h"
#include "video/texture.h"
#include <string>

using namespace std;

Font::Font(string fontfile, int w, int h) {
    try {
        this->parser = new Parser(fontfile);
    } catch(Exception &ex) {
        RETHROW(ex);
    }
    this->w = w;
    this->h = h;
    LOG("Font \"" << fontfile << "\" loaded.");
}

Font::~Font() {
    delete this->parser;
}


bool Font::get_text_pix_size(int *w, int *h, string text) {
    *h = this->h;
    *w = this->w * text.length();
    return true;
}

Texture* Font::get_text_texture(string text, Color *c) {
    Texture *tex = 0;

    // Text image data w*h, 32bpp
    int tw = this->w * text.length();
    int th = this->h;
    uint32_t data[tw*th];

    // Read text from parser
    int chp = 0;
    int gx,gy;
    uint8_t ch;
    for(unsigned int i = 0; i < text.length(); i++) {
        chp = 8 * (text[i]-32);
        parser->reset(chp);
        for(int y = 0; y < this->h; y++) {
            ch = this->parser->ubyte();
            for(int x = 0; x < this->w; x++) {
                gy = y;
                gx = this->w*i + x;
                data[tw*gy+gx] = (ch | (1 << x)) ? c->rgba() : 0;
            }
        }
    }

    try {
        tex = new Texture(tw, th, 4, (char*)data);
    } catch(Exception &ex) {
        RETHROW(ex);
    }

    return tex;
}
