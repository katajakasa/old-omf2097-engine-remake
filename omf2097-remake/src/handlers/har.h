#pragma once

#include <vector>
#include <string>

#include "animation.h"
#include "harpalette.h"
#include "animationsource.h"

class Har : public AnimationSource {
private:
    std::string file;
    bool parse();

public:
    int robot_number;
    int endurance;
    int power;
    int forward_mov;
    int backward_mov;
    int up_mov;
    int down_mov;
    std::vector<Animation*> anims;

    HarPalette palette;

    Har(std::string file);
    ~Har();
    void printDebug();

    Animation* seek_animation(int anim_id);
    Palette* get_palette(int number);
};
