#pragma once

#include <string>
#include <vector>
#include "tools/parser.h"
#include "palette.h"
#include "image.h"

class Animation {
public:
    class Frame : public Image {
    public:
        int x,y;
        int index;
        int present;

        Frame(int x, int y, int w, int h, int rsize, Parser *parser) : Image(w, h, rsize, parser, Image::SPRITE) {
            this->x = x;
            this->y = y;
        }
        Frame(int w, int h, int rsize, Parser *parser) : Image(w, h, rsize, parser, Image::SPRITE) {
            this->x = 0;
            this->y = 0;
        }
    };

	Animation() {}
	~Animation() {}
	void parse(Parser *parser, Palette *palette);
	void printDebug();

	bool has_frame(int frame_no);
	int find_str_descriptor_pos(int frame_no);

    int anim_no;

	// Generic attributes
    std::vector<int> overlays;
    std::string header_string;
    std::string anim_string;
    std::vector<std::string> extra_strings;
    std::vector<Frame*> frames;

	// HAR only
    char unknown_footer_data[21];
    char movement_string[21];
    std::string extra_footer_string;
};
