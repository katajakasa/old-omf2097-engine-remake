#include "menuscene.h"
#include "handlers/background.h"
#include "audio/sampleloader.h"
#include "tools/log.h"

using namespace std;

MenuScene::MenuScene(GLRenderer *renderer, SampleLoader *sloader, Music *music) : Scene("resources/MAIN.BK", renderer, sloader, music) {
    gobj.music = music;
    gobj.renderer = renderer;
    gobj.sloader = sloader;
    for(int i = 0; i < 2; i++) {
        this->player[i] = new AnimationPlayer(gobj, bg, bg->seek_animation(10+i));
    }
    LOG("Initialized.");
}

MenuScene::~MenuScene() {
    for(int i = 0; i < 2; i++) {
        delete this->player[i];
    }
    LOG("Closed.");
}

bool MenuScene::tick(double elapsed) {
    for(int i = 0; i < 2; i++) {
        this->player[i]->tick(elapsed);
    }
    return true;
}

void MenuScene::render() {
    for(int i = 0; i < 2; i++) {
        this->player[i]->render();
    }
}

bool MenuScene::handle_event(sf::Event &event) {
    return true;
}
