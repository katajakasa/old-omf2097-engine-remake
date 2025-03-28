#include "stringengine.h"

#include "tools/log.h"
#include <sstream>
#include <cstdlib>

using namespace std;

int StringEngine::readInt(int *pos, string str) {
    // Find whole integer value
    ostringstream o;
    int start = *pos;
    if(str[*pos] == '-' && (str[(*pos)+1] >= '0' && str[(*pos)+1] <= '9') ) {
        o << str[*pos];
        (*pos)++;
    }
    while(str[*pos] >= '0' && str[*pos] <= '9') {
        o << str[*pos];
        (*pos)++;
    }
    if(o.str().size() == 0)
        return 0;

    // Convert to Int
    int number;
    std::istringstream ss(o.str());
    if((ss >> number).fail()) {
        LOG("Pos(" << start << "-" << pos << "): Failed conversion from " << o.str() << " to " << number);
    }
    return number;
}

void StringEngine::reset_state() {
    bflags = bs = bf = 0;
    bpd = bps = bpn = bpb = bpp = bpf = bpo = tint = 0;
    blend_additive = false;
    sprite_flip_vertical = false;
    sprite_flip_horizontal = false;
    sf = sl = sp = 0;
    fdcode = 0;
    mm = 0;
    mx = my = 0;
    mp = mn = 0;
    sample_id = -1;
    mdel = mcre = -1;
    music_id = -1;
    last_str = "";
    d_set = -1;
    pix = piy = 0xFFFF;
}

StringEngine::StringEngine() {
    this->reset_state();
    px = py = 100;
    pix = piy = 0xFFFF;
    srand(25345464);
}

int StringEngine::parse(string d, int start_p) {
    // Set the initial state
    this->reset_state();

    // Start parsing
    int pos = start_p;
    char c;
    while(pos < (int)d.length()) {
        c = d[pos++];

        // If thia ia a frame rendering request, do it
        if(c >= 'A' && c <= 'Z') {
            this->frame_sel = c-65;
            this->timeout_sel = readInt(&pos, d);
            continue;
        }

        switch(c) {
        // Frame descriptor change. End frame handling here.
        case '-':
            if(d[pos] >= '0' && d[pos] <= '9') {
                this->fdcode = readInt(&pos, d);
            }
            this->last_str = d.substr(start_p, pos-start_p);
            return pos;
            break;

        case 'a': break;
        case 'c': break;
        case 'd':
            this->d_set = readInt(&pos, d);
            break;
        case 'e': break;

        // Rendering
        case 'f': this->sprite_flip_vertical = !this->sprite_flip_vertical; break;
        case 'b':
            c = d[pos++];
            switch(c) {
            case 'z': this->tint = readInt(&pos, d); break;
            case 'r': this->blend_additive = true; break;

            // Rendering methods
            case '1': this->bflags |= 0x2000; break;
            case '2': this->bflags |= 0x4000; break;
            case 'b': this->bflags |= 0x0010; this->bf = readInt(&pos, d); break;
            case 'e': this->bflags |= 0x0800; this->bf = readInt(&pos, d); break;
            case 'f': this->bflags |= 0x0001; this->bf = readInt(&pos, d); break;
            case 'h': this->bflags |= 0x0040; break;
            case 'j': this->bflags |= 0x0400; this->bf = readInt(&pos, d); break;
            case 'l': this->bflags |= 0x0008; this->bf = readInt(&pos, d); break;
            case 'm': this->bflags |= 0x0100; this->bf = readInt(&pos, d); break;
            case 's': this->bs = readInt(&pos, d); break;
            case 'u': this->bflags |= 0x8000; break;
            case 'w': this->bflags |= 0x0080; break;
            case 'x': this->bflags |= 0x0002; break;

            // Palette crap
            case 'p':
                c = d[pos++];
                switch(c) {
                case 'd': this->bpd = readInt(&pos, d); break;
                case 's': this->bps = readInt(&pos, d); break;
                case 'n': this->bpn = readInt(&pos, d); break;
                case 'f': this->bpf = readInt(&pos, d); break;
                case 'p': {
                    int v = readInt(&pos, d);
                    this->bpb = v;
                    this->bpp = v;
                    }
                    break;
                case 'b': this->bpb = readInt(&pos, d); break;
                case 'o': this->bpo = readInt(&pos, d); break;
                }
                break;
            }
            break;

        case 'g': break;
        case 'h': break;
        case 'i': break;
        case 'j': break;
        case 'k': readInt(&pos, d); break;
        case 'l': readInt(&pos, d); break;

        // Random codes
        case 'm':
            if(d[pos] >= '0' && d[pos] <= '9') {
                this->mcre = readInt(&pos, d);
                break;
            }

            c = d[pos++];
            switch(c) {
            case 'd': this->mdel = readInt(&pos, d); break;
            case 'm': this->mm = readInt(&pos, d); break;
            case 'x': this->mx = readInt(&pos, d); break;
            case 'y': this->my = readInt(&pos, d); break;

            // Unknown
            case 'n': mn = readInt(&pos, d); break;
            case 'p': mp = readInt(&pos, d); break;
            case 'g': readInt(&pos, d); break;
            case 'i': readInt(&pos, d); break;

            // Random positions
            case 'r':
                c = d[pos++];
                switch(c) {
                    case 'x': this->mx = rand()%(320 - 2 * this->mm) + readInt(&pos, d); break;
                    case 'y': this->my = rand()%(320 - 2 * this->mm) + readInt(&pos, d); break;
                }
                break;
            }
            break;

        case 'n': break;

        case 'o':
            c = d[pos++];
            switch(c) {
                case 'x': this->ox = readInt(&pos, d); break;
                case 'y': this->oy = readInt(&pos, d); break;
            }
            break;

        case 'p': break;
        case 'q': break;
        case 'r': this->sprite_flip_horizontal = !this->sprite_flip_horizontal; break;

        // Sound handling
        case 's':
            if(d[pos] >= '0' && d[pos] <= '9') {
                this->sample_id = readInt(&pos, d);
                break;
            }

            c = d[pos++];
            switch(c) {
            case 'a': break;
            case 'b': readInt(&pos, d); break;
            case 'c': readInt(&pos, d); break;
            case 'd': break;
            case 'e': readInt(&pos, d); break;
            case 'f': sf = readInt(&pos, d); break;
            case 'l': sl = readInt(&pos, d); break;
            case 'm':
                if(d[pos] >= '0' && d[pos] <= '9') {
                    readInt(&pos, d);
                    break;
                }

                c = d[pos++];
                switch(c) {
                    // Music playback
                    case 'o': music_id = readInt(&pos, d); break;
                }
            case 'p': sp = readInt(&pos, d); break;
            case 'v': readInt(&pos, d); break;
            }
            break;

        case 't': break;
        case 'u': break;
        case 'v': break;
        case 'w': break;
        case 'x':
            if(d[pos] >= '0' && d[pos] <= '9') {
                px = readInt(&pos, d);
                break;
            }

            c = d[pos++];
            switch(c) {
                case '+': px += readInt(&pos, d); break;
                case '-': px -= readInt(&pos, d); break;
                case '=': pix = readInt(&pos, d); break;
            }
            break;
        case 'y':
            if(d[pos] >= '0' && d[pos] <= '9') {
                py = readInt(&pos, d);
                break;
            }

            c = d[pos++];
            switch(c) {
                case '+': py += readInt(&pos, d); break;
                case '-': py -= readInt(&pos, d); break;
                case '=': piy = readInt(&pos, d); break;
            }
            break;
        case 'z': break;
        }
    }

    this->last_str = d.substr(start_p, pos-start_p);
    return -1;
}
