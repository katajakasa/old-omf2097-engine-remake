#pragma once

class SampleLoader;
class GLRenderer;
class Music;
namespace sf {
    class Window;
};

class Engine {
private:
    GLRenderer *renderer;
    SampleLoader *sloader;
    int state;

    sf::Window *sfApp;

    Music *music;

    enum SceneStates {
        SNONE = 0,
        INTRO,
        MENU,
        ARENA
    };

public:
    Engine();
    ~Engine();

    void run();
};
