#ifndef MENUSCENE_H
#define MENUSCENE_H

#include "scene.h"
#include "handlers/animationplayer.h"
#include <string>

class SampleLoader;

class MenuScene : public Scene {
private:
    AnimationPlayer *player[2];
    AnimationPlayer::GameObjects gobj;

public:
    MenuScene(GLRenderer *renderer, SampleLoader *sloader, Music *music);
    virtual ~MenuScene();

    bool tick(double elapsed);
    void render();
    bool handle_event(sf::Event &event);
};

#endif // MENUSCENE_H
