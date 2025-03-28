#pragma once

#include <map>
#include <vector>
#include <string>
#include <cstring>

#include "palette.h"
#include "animation.h"
#include "image.h"
#include "animationsource.h"

class Background : public AnimationSource {
private:
    std::string file;
    bool parse();

public:
    int id;
    int w,h;
    Image *image;

    std::vector<Palette*> palettes;
    std::vector<Animation*> anims;

    Background(std::string file);
    ~Background();

    void printDebug();

    Animation* seek_animation(int anim_id);
    Palette* get_palette(int number);
};
