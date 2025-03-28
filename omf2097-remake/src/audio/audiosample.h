#ifndef SAMPLERATE_H
#define SAMPLERATE_H

#include <SFML/Audio.hpp>

class AudioSample {
private:
    sf::SoundBuffer buffer;
    sf::Sound sound;
    int rate;

public:
    AudioSample(int16_t* data, unsigned int rawlen, int rate, int channels);

    void play();
    void playRate(int changeby, int panning = 0, int volume = 100);
};

#endif // SAMPLERATE_H
