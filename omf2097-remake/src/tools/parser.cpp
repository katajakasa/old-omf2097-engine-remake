#include "parser.h"
#include "tools/exception.h"

using namespace std;

Parser::Parser(string inputfile) {
    fb.open(inputfile.c_str(), ios::in|ios::binary);

    if(fb.is_open()) {
        this->stream = new std::istream(&fb);
        this->stream->seekg(0, ios::end);
        this->filesize = this->stream->tellg();
        this->stream->seekg(0, ios::beg);
    } else {
        THROW("Error while opening file '" << inputfile << "'");
    }
}

Parser::Parser(char* inputdata, int len) {
    this->filesize = len;
    this->sb.sputn(inputdata, len);
    this->stream = new std::istream(&sb);
}

Parser::~Parser() {
    delete this->stream;
}

void Parser::reset() {
    this->stream->seekg(ios::beg);
}

void Parser::reset(unsigned int pos) {
    this->stream->seekg(pos, ios::beg);
}

int8_t Parser::byte() {
    int8_t tv = 0;
    stream->read((char*)(&tv), 1);
    return tv;
}

int16_t Parser::word() {
    int16_t tv = 0;
    stream->read((char*)(&tv), 2);
    return tv;
}

int32_t Parser::int3() {
    char s[3];
    int32_t v = 0;
    stream->read(s, 3);
    for(int i = 2; i >= 0; i--) {
        v <<= 8;
        v |= s[i] & 0xFF;
    }
    return v;
}

int32_t Parser::dword() {
    int32_t v = 0;
    stream->read((char*)(&v), 4);
    return v;
}

uint8_t Parser::ubyte() {
    uint8_t tv = 0;
    stream->read((char*)(&tv), 1);
    return tv;
}

uint16_t Parser::uword() {
    uint16_t tv = 0;
    stream->read((char*)(&tv), 2);
    return tv;
}

uint32_t Parser::uint3() {
    char s[3];
    uint32_t v = 0;
    stream->read(s, 3);
    for(int i = 2; i >= 0; i--) {
        v <<= 8;
        v |= s[i] & 0xFF;
    }
    return v;
}

uint32_t Parser::udword() {
    int32_t v = 0;
    stream->read((char*)(&v), 4);
    return v;
}

char Parser::rchar() {
    char c = 0;
    stream->read((char*)(&c), 1);
    return c;
}

char Parser::peek_byte() {
    return stream->peek();
}

uint32_t Parser::peek_udword() {
    uint32_t v = this->udword();
    stream->seekg(this->getpos()-4, ios::beg);
    return v;
}

std::string Parser::str(int len) {
    char data[len];
    this->data(data, len);
    return std::string(data, len);
}

std::string Parser::cstr(int len) {
    char data[len];
    this->data(data, len);
    return std::string(data);
}

void Parser::data(char* output, int len) {
    stream->read(output, len);
}

void Parser::pass(int len) {
    stream->seekg(len, ios::cur);
}

void Parser::passndump(int len) {
    for(int i = 0; i < len; i++) {
        uint8_t byte = this->ubyte();
        printf("%.2x ", byte);
    }
}
