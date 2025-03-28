#ifndef ANIMATION_H
#define ANIMATION_H

#include <vector>
#include <string>
#include "jaguarleap/shared/sprite.h"

class Parser;
class Writer;

struct AnimationHeader {
    char padding[8];
    uint16_t overlay_count;
    uint8_t frame_count;
    std::vector<uint32_t> overlays;
};

struct AnimationExtraString {
    uint16_t length;
    std::string extra_str;
    uint8_t null;
};

struct AnimationStringHeader {
    uint16_t length;
    std::string anim_str;
    uint8_t null;
    uint8_t extrastr_count;
    std::vector<AnimationExtraString> extra_strings;
};

class Animation {
public:
    AnimationHeader header;
    AnimationStringHeader strings;
    std::vector<Sprite*> sprites;

    Animation(Parser &p);
    ~Animation();
    bool save(Writer &w);
};

#endif // ANIMATION_H
