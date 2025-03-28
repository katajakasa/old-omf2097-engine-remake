#pragma once

#include <string>
#include <fstream>
#include <inttypes.h>
#include <iostream>

class TGAWriter {
private:
    std::ofstream *off;
    bool isopen;

public:
    TGAWriter(std::string filename, int w, int h);
    ~TGAWriter();

    void write(uint8_t r, uint8_t g, uint8_t b);
    void write(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    void write(char* data, int len);
    void finish();
};
