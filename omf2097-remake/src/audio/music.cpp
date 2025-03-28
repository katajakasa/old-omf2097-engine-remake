#include "music.h"

#include <fstream>
#include "tools/exception.h"
#include "tools/log.h"

using namespace std;

void Music::close() {
    // Stop old playback
    if(started) {
        this->Stop();
        duh_end_sigrenderer(this->renderer);
        this->renderer = 0;
        this->started = false;
    }

    // Unload old file
    if(this->opened) {
        unload_duh(this->data);
    }
}

bool Music::load(int file) {
    this->close();

    // Get filename
    string filename;
    switch(file) {
        case ARENA0: filename = "resources/ARENA0.PSM"; break;
        case ARENA1: filename = "resources/ARENA1.PSM"; break;
        case ARENA2: filename = "resources/ARENA2.PSM"; break;
        case ARENA3: filename = "resources/ARENA3.PSM"; break;
        case ARENA4: filename = "resources/ARENA4.PSM"; break;
        case END:  filename = "resources/END.PSM"; break;
        case MENU: filename = "resources/MENU.PSM"; break;
    }

    // Read contents
    this->data = dumb_load_psm(filename.c_str(), 0);
    if(!this->data) {
        THROW("Error while loading PSM file!");
    }

    this->opened = true;

    LOG("Libdumb initialized, " << filename << " open!");
    return true;
}

Music::Music() {
    // Audio settings
    this->channels = 2;
    this->bits = 16;
    this->frequency = 44100;
    this->delta = 65536.0f / this->frequency;

    // Init SFML Audio Subsystem
    Initialize(this->channels, this->frequency);

    this->renderer = 0;
    this->started = false;
    this->opened = false;
    LOG("Music renderer loaded!");
}

Music::~Music() {
    this->close();
    LOG("Music renderer unloaded.");
}

bool Music::OnStart() {
    if(!this->opened) {
        LOG("Cannot start playback: not music file loaded!");
        return false;
    }
    if(started) {
        duh_end_sigrenderer(this->renderer);
    }
    this->renderer = duh_start_sigrenderer(this->data, 0, this->channels, 0);
    this->started = true;
    return true;
}

bool Music::OnGetData(Chunk& Data) {
    duh_render(this->renderer, this->bits, 0, 1.0f, this->delta, 8192, this->buffer);
    Data.Samples = this->buffer;
    Data.NbSamples = 16384;
    return true;
}
