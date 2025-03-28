#include "audiosample.h"

#include <string>
#include "tools/exception.h"

using namespace std;

AudioSample::AudioSample(int16_t* data, unsigned int rawlen, int rate, int channels) {
    if (!buffer.LoadFromSamples(data, rawlen, channels, rate)) {
        THROW("Error while loading sample.");
    }
    this->rate = rate;
    sound.SetBuffer(buffer);
    sound.SetLoop(false);
}

/**
  * Play sample at default panning and volume. Convert and play at output samplerate.
  */
void AudioSample::play() {
    this->playRate(0);
}

void AudioSample::playRate(int changeby, int panning, int volume) {
    sound.SetPitch(((float)this->rate / ((float)this->rate+changeby)));
    sound.SetVolume(volume);

    if(panning != 0) {
        sound.SetRelativeToListener(true);
        sound.SetPosition(0.3*(panning-10), 0.0f, 1.0f);
    }

    this->sound.Play();
}
