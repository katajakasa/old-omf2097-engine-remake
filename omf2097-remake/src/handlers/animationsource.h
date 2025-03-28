#ifndef ANIMATIONSOURCE_H
#define ANIMATIONSOURCE_H

class Animation;
class Palette;

class AnimationSource {
public:
    virtual Animation* seek_animation(int anim_id) = 0;
    virtual Palette* get_palette(int number) = 0;
};

#endif // ANIMATIONSOURCE_H
