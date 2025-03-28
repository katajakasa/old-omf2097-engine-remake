#pragma once

#include <string>
#include <dumb/dumb.h>
#include <SFML/Audio.hpp>

class Music : public sf::SoundStream {
private:
    DUH *data;
    DUH_SIGRENDERER *renderer;

protected:
    bool OnStart();
    bool OnGetData(Chunk& Data);
    int16_t buffer[16384];

    void close();

public:
    enum MusicFileIdentifier {
        END = 1,
        MENU,
        ARENA0,
        ARENA1,
        ARENA2,
        ARENA3,
        ARENA4
    };

    bool started, opened;

    int channels, bits, frequency;
    float delta;

    bool load(int file);
    Music();
    ~Music();
};
