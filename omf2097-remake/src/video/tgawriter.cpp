#include "tgawriter.h"

using namespace std;

TGAWriter::TGAWriter(std::string filename, int w, int h) {
    this->off = new ofstream(filename.c_str());
    if(off->is_open()) {
        this->isopen = true;
        // Init for 32bit data, origin 0,0, size (w,h)
        (*off) << (uint8_t) 0;
        (*off) << (uint8_t) 0;
        (*off) << (uint8_t) 2;
        for(int i = 0; i < 9; i++)
            (*off) << (uint8_t) 0;
        (*off) << (uint8_t)(w & 0x00FF);
        (*off) << (uint8_t)((w & 0xFF00) / 256);
        (*off) << (uint8_t)(h & 0x00FF);
        (*off) << (uint8_t)((h & 0xFF00) / 256);
        (*off) << (uint8_t) 32;
        (*off) << (uint8_t) 0;
    }
}

TGAWriter::~TGAWriter() {
    if(this->isopen) {
        this->finish();
    }
    if(this->off) {
        delete this->off;
    }
}

void TGAWriter::write(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    if(!this->isopen) return;
    (*off) << b << g << r << a;
}

void TGAWriter::write(uint8_t r, uint8_t g, uint8_t b) {
    if(!this->isopen) return;
    (*off) << b << g << r << (uint8_t)0;
}

void TGAWriter::write(char* data, int len) {
    if(!this->isopen) return;
    off->write(data, len);
}

void TGAWriter::finish() {
    this->off->close();
}
