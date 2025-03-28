#include "jaguarleap/shared/animation.h"
#include "jaguarleap/fileops/parser.h"
#include "jaguarleap/fileops/writer.h"
#include "jaguarleap/shared/parserexception.h"

Animation::~Animation() {
    for(auto *s : this->sprites) {
        delete s;
    }
}

Animation::Animation(Parser &p) {
    // Read animation header
    p.data(header.padding, 8);
    header.overlay_count = p.uword();
    header.frame_count = p.ubyte();
    for(int i = 0; i < header.overlay_count; i++)
        header.overlays.push_back(p.udword());

    // Read animation string header
    strings.length = p.uword();
    strings.anim_str = p.str(strings.length);
    strings.null = p.ubyte();
    strings.extrastr_count = p.ubyte();

    // Read extra strings
    for(int i = 0; i < strings.extrastr_count; i++) {
        AnimationExtraString es;
        es.length = p.uword();
        es.extra_str = p.str(es.length);
        es.null = p.ubyte();
        strings.extra_strings.push_back(es);
    }

    // Read Sprites
    try {
        for(int i = 0; i < header.frame_count; i++) {
            this->sprites.push_back(new Sprite(p));
        }
    } catch(ParserException &ex) {
        throw ex;
    }
}

bool Animation::save(Writer &w) {
    // Write animation header
    w.data(header.padding, 8);
    w.uword(header.overlay_count);
    w.ubyte(header.frame_count);
    for(int32_t overlay : header.overlays) {
        w.udword(overlay);
    }

    // Write animation string header
    w.uword(strings.length);
    w.data(strings.anim_str.c_str(), strings.anim_str.length());
    w.ubyte(strings.null);
    w.ubyte(strings.extrastr_count);

    // Write extra strings
    for(auto &es : strings.extra_strings) {
        w.uword(es.length);
        w.data(es.extra_str.c_str(), es.extra_str.length());
        w.ubyte(es.null);
    }

    // Write sprites
    for(auto *sp : sprites) {
        sp->save(w);
    }

    return true;
}
