#include "introscene.h"

#include "tools/log.h"

using namespace std;

/**
  * This fixes the positioning of the first two text frames in intro
  */
void IntroFrameFix::fix(int anim_no, int frame_no, Animation::Frame *frame) {
    if(anim_no == 5 && frame_no == 0) {
        frame->y = 60;
    }
    if(anim_no == 6 && frame_no == 0) {
        frame->y = 50;
    }
}

IntroScene::IntroScene(GLRenderer *renderer, SampleLoader *sloader, Music *music) : Scene("resources/INTRO.BK", renderer, sloader, music) {
    this->gobj.renderer = renderer;
    this->gobj.music = music;
    this->gobj.sloader = sloader;
    this->player = new AnimationPlayer(this->gobj, this->bg, this->bg->seek_animation(25));
    this->player->set_int_xy(160, 0);
    this->player->set_int_np(0, 101);
    this->player->set_frame_fixer(new IntroFrameFix());
    LOG("Initialized.");
}

IntroScene::~IntroScene() {
    delete this->player;
    LOG("Closed.");
}

bool IntroScene::tick(double elapsed) {
    if(this->player == 0) {
        return false;
    }

    if(!this->player->tick(elapsed)) {
        delete this->player;
        this->player = 0;
        return false;
    }
    return true;
}

void IntroScene::render() {
    this->player->render();
}
