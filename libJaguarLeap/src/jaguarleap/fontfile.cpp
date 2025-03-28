#include "jaguarleap/fontfile.h"
#include "jaguarleap/fileops/parser.h"
#include "jaguarleap/fileops/writer.h"
#include "jaguarleap/shared/parserexception.h"
#include <cstring>

FontFile::FontFile(std::string file, FontType type) {
    Parser p(file);

    this->_w = this->getWidth(type);
    this->_h = 8;
    this->len = p.getfilesize();
    this->data = new char[this->len];
    p.data(this->data, this->len);
}

FontFile::~FontFile() {
    delete[] this->data;
}

int FontFile::getWidth(FontType type) {
    switch(type) {
        case SMALL: return 6;
        case LARGE: return 8;
    }
    return -1;
}

void FontFile::save(std::string filename) {
    Writer w;
    w.data(this->data, this->len);

    // Save file
    if(!w.save(filename)) {
        throw ParserException("Error while saving to file");
    }
}

char* FontFile::get(char c) {
    if(c < 32) {
        throw ParserException("Requested character does not exist in font!");
    }
    return this->data + this->w() * (c - 32);
}

void FontFile::set(char c, char* data) {
    if(c < 32) {
        throw ParserException("Requested character does not exist in font!");
    }
    memcpy(get(c), data, this->w());
}
