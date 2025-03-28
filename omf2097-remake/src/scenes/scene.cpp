#include "scene.h"

#include "handlers/background.h"
#include "handlers/har.h"
#include "handlers/stringengine.h"
#include "video/texture.h"
#include "tools/exception.h"

using namespace std;

Scene::Scene(string file, GLRenderer *renderer, SampleLoader *sloader, Music *music) {
    this->file = file;
    this->renderer = renderer;
    this->sloader = sloader;
    this->music = music;
    try {
        this->bg = new Background(file.c_str());
        this->bg->printDebug();
    } catch(Exception &ex) {
        RETHROW(ex);
    }
    this->renderer->set_background(this->bg->image->getTexture(bg->palettes.at(0)));
}

Scene::~Scene() {
    this->renderer->set_background(0);
    delete this->bg;
}

void Scene::render() {

}
