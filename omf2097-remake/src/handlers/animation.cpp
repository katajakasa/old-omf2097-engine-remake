#include "animation.h"

#include "tools/log.h"

using namespace std;

/**
  * Parses the animation data
  */
void Animation::parse(Parser* parser, Palette *palette) {
    // Load Animation Header
    parser->pass(8);
    int overlaycount = parser->uword();
    int framecount = parser->ubyte();

    // Read overlays
    for(int i = 0; i < overlaycount; i++) {
        int overlay = parser->udword();
        this->overlays.push_back(overlay);
    }

    // Load String Header
    int strhlen = parser->word();
    this->anim_string = parser->str(strhlen);
    parser->pass(1);
    int num_extra_strings = parser->byte();

    // Read extra strings
    for(int i = 0; i < num_extra_strings; i++) {
        int len = parser->word();
        string extra_str = parser->str(len);
        parser->pass(1);
        this->extra_strings.push_back(extra_str);
    }

    // Load sprites
    for(int m = 0; m < framecount; m++) {
        // Read sprite information
        int spritelen = parser->uword();
        int x = parser->word();
        int y = parser->word();
        int w = parser->word();
        int h = parser->word();
        int index = parser->ubyte();
        int npresent = parser->ubyte();

        // Make sure we're not seeing a faulty frame
        if(w*h == 0) {
            LOG("Faulty frame " << m);
            continue;
        }

        // Create new animation frame
        if(spritelen > 0 && !npresent) {
            Frame *frame = new Frame(x, y, w, h, spritelen, parser);
            frame->index = index;
            frame->present = !npresent;
            this->frames.push_back(frame);
        }

        // Make sure parser is feeling good
        if(!parser->isok()) {
            break;
        }
    }
}

/**
  * Check if animation frame exists
  */
bool Animation::has_frame(int anim_no) {
    return (anim_no < (int)this->frames.size());
}

/**
  * Finds the position of the first character of a frame descriptor in the animation string
  */
int Animation::find_str_descriptor_pos(int frame_no) {
    int pos = -2;
    int r = 0;
    while((pos = this->anim_string.find_first_of('-', pos+1)) != std::string::npos && ++r < frame_no);
    return pos+1;
}

/**
  * Prints debug information for the Animation. Fugly, but works.
  */
void Animation::printDebug() {
    LOG("  (" << this->anim_no << ") Animation");

    if(this->header_string.size() > 0)
        LOG("      Animation header (" << this->header_string.size() << "): " << this->header_string);

    LOG("      String header (" << this->anim_string.size() << "): " << this->anim_string);

    if(this->extra_strings.size() > 0) {
        LOG("      Extra strings (" << this->extra_strings.size() << "): ");
        for(unsigned int i = 0; i < this->extra_strings.size(); i++) {
            LOG("       * " << this->extra_strings.at(i));
        }
    }

    LOG("      String footer (" << this->extra_footer_string.size() << "): " << this->extra_footer_string);

    LOG("      Images (" << this->frames.size() << "): ");
    for(unsigned int i = 0; i < this->frames.size(); i++) {
        Frame *frame = this->frames.at(i);
        LOG("     "
             << " (" << (char)(i+'A') << ") Frame: Length=" << frame->size()
             << " (x,y)=(" << frame->x << "," << frame->y << ")"
             << " (w,h)=(" << frame->w << "," << frame->h << ")"
             << " Index=" << frame->index
             << " Present=" << frame->present);
    }
}
