#ifndef INTROSCENE_H
#define INTROSCENE_H

#include "scene.h"
#include "handlers/animationplayer.h"
#include "handlers/background.h"

class Animation;
class SampleLoader;
class Music;

class IntroFrameFix : public AnimationPlayer::FrameFix {
public:
    void fix(int anim_no, int frame_no, Animation::Frame *frame);
};

class IntroScene : public Scene {
private:
    // Root player
    AnimationPlayer *player;

    // Running bool + scene global ticks
    bool run;

    // Some global stuff
    AnimationPlayer::GameObjects gobj;

public:
    IntroScene(GLRenderer *renderer, SampleLoader *sloader, Music *music);
    virtual ~IntroScene();

    bool tick(double elapsed);
    void render();
};

#endif // INTROSCENE_H
