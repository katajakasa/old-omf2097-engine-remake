#include "background.h"

#include "tools/parser.h"
#include "tools/exception.h"
#include "tools/log.h"
#include "palette.h"

using namespace std;

Background::Background(string file) {
    this->file = file;
    this->image = 0;
    if(!this->parse()) {
        THROW("Error while trying to parse " << file << "!");
    }
}

Background::~Background() {
    if(this->image)
        delete this->image;
}

bool Background::parse() {
    Parser parser(file);
    if(!parser.isok()) {
        return false;
    }

    // Read general information
    this->id = parser.dword();
    parser.pass(1);
    this->w = parser.word();
    this->h = parser.word();

    // Some helpful vars ...
    bool run = true;
    unsigned int n_offset = 9;
    unsigned int tmp_offset = 0;

    // Find palettes
    while(run) {
        tmp_offset = parser.udword();
        if(n_offset == tmp_offset) {
            run = false;
            continue;
        }
        parser.reset(tmp_offset);
        n_offset = tmp_offset;
    }

    // Skip background data
    parser.pass(1);
    parser.pass(this->w*this->h);

    // Read palettes
    int palettecount = parser.byte();
    for(int i = 0; i < palettecount; i++) {
        Palette *p = new Palette;
        for(int g = 0; g < 256; g++) {
            p->palette[g][0] = (uint8_t)(parser.ubyte() / 63.0 * 255);
            p->palette[g][1] = (uint8_t)(parser.ubyte() / 63.0 * 255);
            p->palette[g][2] = (uint8_t)(parser.ubyte() / 63.0 * 255);
        }
        parser.data((char*)p->remaps, 4864);
        this->palettes.push_back(p);
    }

    // Read animations
    run = true;
    n_offset = 9;
    tmp_offset = 0;
    while(run) {
        parser.reset(n_offset);
        Animation *anim = new Animation;

        // Find next offset. If next == this one, stop.
        tmp_offset = parser.udword();
        if(n_offset == tmp_offset) {
            run = false;
            continue;
        }
        n_offset = tmp_offset;

        // Parse animation number & header string
        anim->anim_no = parser.byte();
        parser.pass(7);
        int hslen = parser.uword();
        anim->header_string = parser.str(hslen);

		// Parse generic animation
		anim->parse(&parser, this->palettes.at(0));
		this->anims.push_back(anim);
	}

    // Read background image
    this->image = new Image(this->w, this->h, this->w*this->h, &parser, Image::VGA);
    //this->image->writeTGA(this->palettes.at(0), "data/background.tga");

    // Palettes. There have already been read, so let's just pass.
    parser.pass(1);
    parser.pass(palettecount*22*256);

    // Footer. Dunno what this is. 30 bytes. Handle for kicks :P
    for(int i = 0; i < 3; i++) {
        parser.pass(10);
    }

    return true;
}

Animation* Background::seek_animation(int anim_id) {
    for(unsigned int i = 0; i < anims.size(); i++) {
        if(anims.at(i)->anim_no == anim_id) {
            return anims.at(i);
        }
    }
    return 0;
}

Palette* Background::get_palette(int number) {
    return this->palettes.at(number);
}

void Background::printDebug() {
    LOG("[Background]");
    LOG("  Name: " << this->file);
    LOG("  Size: " << this->w << "x" << this->h << "px");
    LOG("  ID: " << this->id);
    LOG("  Got " << this->palettes.size() << " palette(s).");
    LOG("  Got " << this->image->size() << " bytes of background data.");
    LOG("  Animations (" << this->anims.size() << "):");
    for(unsigned int i = 0; i < this->anims.size(); i++) {
        this->anims.at(i)->printDebug();
        /*for(unsigned int k = 0; k < this->anims.at(i)->frames.size(); k++) {
            Animation::Frame *f = this->anims.at(i)->frames.at(k);
            ostringstream o;
            o << "data/spr_" << ((char)(65+anims.at(i)->anim_no)) << "_" << k << ".tga";
            f->writeTGA(this->palettes.at(0), o.str());
        }*/
    }
    LOG("[/Background]");
}
