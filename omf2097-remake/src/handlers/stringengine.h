#ifndef STRINGENGINE_H
#define STRINGENGINE_H

#include <string>

class StringEngine {
protected:
    int readInt(int* pos, std::string str);
    void reset_state();

public:
    StringEngine();

    // Frame select & timeout
    int frame_sel, timeout_sel;

    // Palette state
    int bpd, bps, bpn, bpb, bpp, bpf, bpo, tint;

    // Rendering method
    int bf, bs, bflags;
    bool blend_additive;

    // Sprites
    bool sprite_flip_vertical;
    bool sprite_flip_horizontal;

    // Animations
    int mdel, mcre;

    // Animation frame selection
    int d_set;

    // Other stuff
    int fdcode;

    // Sound
    int sf, sl, sp, sample_id;

    // Music
    int music_id;

    // positioning
    int mm, mx, my;
    int ox, oy;
    int x,y;
    int mp, mn;

    int px, py;
    int pix, piy;

    // For debugging purposes
    std::string last_str;

    // Methods
    int parse(std::string d, int start_p = 0);
};

#endif // STRINGENGINE_H
