#pragma once

#include <vector>
#include <string>

#include "video/glrenderer.h"
#include "video/texture.h"
#include "handlers/stringengine.h"
#include "handlers/image.h"
#include "audio/sampleloader.h"
#include "audio/music.h"
#include <SFML/Window.hpp>

class Background;

class Scene {
private:
    std::string file;

protected:
    GLRenderer *renderer;
    Background *bg;
    SampleLoader *sloader;
    Music *music;

public:
    Scene(std::string file, GLRenderer *renderer, SampleLoader *sloader, Music *music);
    virtual ~Scene();

    virtual bool tick(double elapsed) { return true; }
    virtual void render() = 0;
    virtual bool handle_event(sf::Event &event) { return false; }
};
