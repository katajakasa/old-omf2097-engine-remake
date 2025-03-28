#include "engine.h"

#include <SFML/Window.hpp>

#include "video/glrenderer.h"
#include "audio/sampleloader.h"
#include "audio/audiosample.h"
#include "audio/music.h"
#include "handlers/har.h"
#include "scenes/scene.h"
#include "scenes/menuscene.h"
#include "scenes/introscene.h"
#include "tools/exception.h"

using namespace std;

Engine::Engine() {
    this->sfApp = new sf::Window;
    try {
        this->renderer = new GLRenderer(this->sfApp, 640, 400, false);
        this->sloader = new SampleLoader();
        this->music = new Music();
    } catch(Exception &ex) {
        RETHROW(ex);
    }

}

Engine::~Engine() {
    delete this->renderer;
    delete this->sloader;
    delete this->music;
}

void Engine::run() {
    Scene *scene = 0;
    int state = 0;
    bool run = true;
    double elapsed = 0.0;

    state = INTRO;
    scene = new IntroScene(renderer, sloader, music);

    //state = MENU;
    //scene = new MenuScene(renderer, sloader);

    // Run engine
    sf::Clock clock;
    while(run) {
        clock.Reset();

        // Poll keys
        sf::Event event;
        while(sfApp->GetEvent(event)) {
            // Check if Scene does something with the event.
            /*if(scene) {
                if(scene->handle_event(event)) {
                    continue;
                }
            }*/

            // Scene didn't handle the event, let's try these ...
            if(event.Type == sf::Event::Closed)
                run = false;
            if((event.Type == sf::Event::KeyPressed) && (event.Key.Code == sf::Key::Escape)) {
                if(state == INTRO) {
                    delete scene;
                    scene = new MenuScene(renderer, sloader, music);
                    state = MENU;
                    if(!this->music->started) {
                        this->music->load(Music::MENU);
                        this->music->Play();
                    }
                } else {
                    run = false;
                }
            }
            /*
            if((event.Type == sf::Event::KeyPressed) && (event.Key.Code == sf::Key::A)) {
                AudioSample* s = this->sloader->getSample(14);
                s->play();
            }
            */
        }

        // Handle rendering
        renderer->prepare();
        if(!scene->tick(elapsed)) {
            switch(state) {
            case INTRO: // INTRO done, Set MENU
                delete scene;
                scene = new MenuScene(renderer, sloader, music);
                state = MENU;
                break;
            case MENU:
            case ARENA:
            default:
                run = false;
                break;
            }
        } else {
            scene->render();
            renderer->finish();
        }

        // Wait a moment and see how fast we are going.
        elapsed = clock.GetElapsedTime();
    }

    // Destroy scene, if any are running.
    if(scene) delete scene;
}
