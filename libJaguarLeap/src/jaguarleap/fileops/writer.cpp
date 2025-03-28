#include "jaguarleap/fileops/writer.h"
#include "jaguarleap/shared/parserexception.h"
#include <cstring>
#include <fstream>
#include <iostream>

Writer::Writer() {
    this->d = 0;
    this->clear();
}

Writer::~Writer() {
    if(this->d != 0) {
        delete[] this->d;
    }
}

void Writer::copy(Writer &source) {
    this->data(source.d, source.offset);
}

void Writer::data(const char* d, int len) {
    // Resize data array if necessary
    if(this->offset + len > this->max_size) {
        int nsize = this->max_size + 1024 + len;
        char* ndata = new char[nsize];
        memcpy(ndata, this->d, this->offset);
        delete[] this->d;
        this->d = ndata;
        this->max_size = nsize;
    }

    // Write data
    memcpy(this->d + this->offset, d, len);
    this->offset += len;
}

void Writer::cstr(std::string str, unsigned int len) {
    if(str.length() > len) {
        throw ParserException("Cannot write; string too long!");
    }

    char b[len];
    memset(b, 0, len);
    memcpy(b, str.c_str(), str.length());
    this->data(b, len);
}

void Writer::str(std::string str) {
    this->data(str.c_str(), str.length());
}

void Writer::clear() {
    if(this->d != 0) {
        delete[] this->d;
    }
    this->d = new char[8192];
    this->offset = 0;
    this->max_size = 8192;
}

bool Writer::save(std::string filename) {
    std::ofstream f;
    f.open(filename.c_str(), std::ios::out|std::ios::binary);
    if(!f.is_open()) {
        return false;
    }
    f.write(this->d, this->offset);
    f.close();
    return true;
}
