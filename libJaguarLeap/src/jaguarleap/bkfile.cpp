#include "jaguarleap/bkfile.h"
#include "jaguarleap/fileops/parser.h"
#include "jaguarleap/fileops/writer.h"
#include "jaguarleap/shared/parserexception.h"
#include <cstring>

void BKImage::convertToRGB(char* output, Palette &pal, int remap) {
    int w = 320;
    int h = 200;
    memset(output, 0xFF, w*h*3);
    int pos = 0;
    for(int y = h-1; y >= 0; y--) {
        for(int x = 0; x < w; x++) {
            uint8_t b = this->data[y * w + x];
            pos = ((y * w) + x)*3;
            if(remap > -1) {
                output[pos+0] = (uint8_t)pal.data[(uint8_t)pal.remaps[remap][b]][0];
                output[pos+1] = (uint8_t)pal.data[(uint8_t)pal.remaps[remap][b]][1];
                output[pos+2] = (uint8_t)pal.data[(uint8_t)pal.remaps[remap][b]][2];
            } else {
                output[pos+0] = (uint8_t)pal.data[b][0];
                output[pos+1] = (uint8_t)pal.data[b][1];
                output[pos+2] = (uint8_t)pal.data[b][2];
            }
        }
    }
}

BKFile::~BKFile() {
    for(auto &a : anims) {
        delete a.anim;
    }
}

BKFile::BKFile(std::string filename) {
    Parser p(filename);

    // Read header
    header.id = p.udword();
    header.unknown_a = p.ubyte();
    header.img_w = p.uword();
    header.img_h = p.uword();

    // Read animations
    int8_t animno = 0;
    while(true) {
        // Get offset & animation number
        p.pass(4);
        animno = p.ubyte();
        if(animno >= 50 || !p.isok()) {
            this->last_animno = animno;
            break;
        }

        // Read animation header structure
        BKAnimation a;
        a.id = animno;
        a.unknown_a = p.ubyte();
        a.unknown_b = p.ubyte();
        a.unknown_c = p.ubyte();
        a.unknown_d = p.ubyte();
        a.unknown_e = p.uword();
        a.unknown_f = p.ubyte();
        a.unknown_g = p.uword();
        a.unknown_h = p.str(a.unknown_g);

        // ... and parse it.

        try {
             a.anim = new Animation(p);
        } catch(ParserException &ex) {
            throw ex;
        }

        // Save animation
        this->anims.push_back(a);
    }

    // Read background image
    p.data(footer.img.data, 320*200);

    // Read palettes & remaps
    uint8_t num_palettes = p.ubyte();
    for(uint8_t i = 0; i < num_palettes; i++) {
        Palette palette;
        p.data((char*)palette.data, 256*3);
        p.data((char*)palette.remaps, 19*256);
        footer.palettes.push_back(palette);
    }

    // Read text footer (TODO: What is this ?)
    p.data(footer.unknown, 30);
}

void BKFile::save(std::string filename) {
    Writer w;

    // Write header
    w.udword(header.id);
    w.ubyte(header.unknown_a);
    w.uword(header.img_w);
    w.uword(header.img_h);

    // Write animations
    uint32_t next_animation_start = w.len();
    Writer subwa, subwb;
    for(auto &a : this->anims) {
        // Write animation to subwriter a
        a.anim->save(subwa);

        // Write header to another writer
        subwb.ubyte(a.id);
        subwb.ubyte(a.unknown_a);
        subwb.ubyte(a.unknown_b);
        subwb.ubyte(a.unknown_c);
        subwb.ubyte(a.unknown_d);
        subwb.uword(a.unknown_e);
        subwb.ubyte(a.unknown_f);
        subwb.uword(a.unknown_g);
        subwb.data(a.unknown_h.c_str(), a.unknown_h.length());

        // Calculate position for next animation
        next_animation_start = w.len() + subwa.len() + subwb.len() + 4;

        // Write the whole animation
        w.udword(next_animation_start);
        w.copy(subwb);
        w.copy(subwa);

        // Clear writers
        subwa.clear();
        subwb.clear();
    }

    // Write last animation position marker + anim number
    w.udword(next_animation_start);
    w.ubyte(this->last_animno);

    // Write image
    w.data(footer.img.data, 320*200);

    // Write palettes
    w.ubyte(this->footer.palettes.size());
    for(auto &palette : this->footer.palettes) {
        w.data((char*)palette.data, 256*3);
        w.data((char*)palette.remaps, 19*256);
    }

    // Write footer data
    w.data(footer.unknown, 30);

    // Save file
    if(!w.save(filename)) {
        throw ParserException("Error while saving to file");
    }
}

