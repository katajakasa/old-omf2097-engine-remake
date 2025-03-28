#include "jaguarleap/fileops/parser.h"
#include "jaguarleap/shared/parserexception.h"
#include <fstream>
#include <sstream>

using namespace std;

Parser::Parser() {
    this->stream = 0;
    this->fb = 0;
    this->sb = 0;
}

Parser::Parser(std::string inputfile) {
    this->stream = 0;
    this->fb = 0;
    this->sb = 0;
    if(!this->open(inputfile)) {
        throw ParserException("Error while opening file!");
    }
}

Parser::Parser(const char* inputdata, int len) {
    this->stream = 0;
    this->fb = 0;
    this->sb = 0;
    this->filesize = len;
    this->sb = new stringbuf();
    this->sb->sputn(inputdata, len);
    this->stream = new istream(this->sb);
}

Parser::~Parser() {
    this->close();
}

void Parser::close() {
    if(this->stream) {
        delete this->stream;
        this->stream = 0;
    }
    if(this->fb) {
        delete this->fb;
        this->fb = 0;
    }
    if(this->sb) {
        delete this->sb;
        this->sb = 0;
    }
}

bool Parser::open(std::string inputfile) {
    this->fb = new filebuf();
    this->fb->open(inputfile.c_str(), ios::in|ios::binary);

    if(this->fb->is_open()) {
        this->stream = new istream(this->fb);
        this->stream->seekg(0, ios::end);
        this->filesize = this->stream->tellg();
        this->stream->seekg(0, ios::beg);
    } else {
        return false;
    }
    return true;
}

bool Parser::isok() {
    if(!this->stream) throw ParserException("Must open file for parsing first!");
    return !this->stream->eof();
}

void Parser::reset() {
    if(!this->stream) throw ParserException("Must open file for parsing first!");
    this->stream->seekg(ios::beg);
}

void Parser::reset(unsigned int pos) {
    if(!this->stream) throw ParserException("Must open file for parsing first!");
    this->stream->seekg(pos, ios::beg);
}

int8_t Parser::byte() {
    if(!this->stream) throw ParserException("Must open file for parsing first!");
    int8_t tv = 0;
    stream->read((char*)(&tv), 1);
    return tv;
}

int16_t Parser::word() {
    if(!this->stream) throw ParserException("Must open file for parsing first!");
    int16_t tv = 0;
    stream->read((char*)(&tv), 2);
    return tv;
}

int32_t Parser::dword() {
    if(!this->stream) throw ParserException("Must open file for parsing first!");
    int32_t v = 0;
    stream->read((char*)(&v), 4);
    return v;
}

uint8_t Parser::ubyte() {
    if(!this->stream) throw ParserException("Must open file for parsing first!");
    uint8_t tv = 0;
    stream->read((char*)(&tv), 1);
    return tv;
}

uint16_t Parser::uword() {
    if(!this->stream) throw ParserException("Must open file for parsing first!");
    uint16_t tv = 0;
    stream->read((char*)(&tv), 2);
    return tv;
}

uint32_t Parser::udword() {
    if(!this->stream) throw ParserException("Must open file for parsing first!");
    uint32_t v = 0;
    stream->read((char*)(&v), 4);
    return v;
}

char Parser::rchar() {
    if(!this->stream) throw ParserException("Must open file for parsing first!");
    char c = 0;
    stream->read((char*)(&c), 1);
    return c;
}

char Parser::peek_byte() {
    if(!this->stream) throw ParserException("Must open file for parsing first!");
    return stream->peek();
}

uint32_t Parser::peek_udword() {
    if(!this->stream) throw ParserException("Must open file for parsing first!");
    uint32_t v = this->udword();
    stream->seekg(this->getpos()-4, ios::beg);
    return v;
}

std::string Parser::str(int len) {
    if(!this->stream) throw ParserException("Must open file for parsing first!");
    char data[len];
    this->data(data, len);
    return std::string(data, len);
}

std::string Parser::cstr(int len) {
    if(!this->stream) throw ParserException("Must open file for parsing first!");
    char data[len];
    this->data(data, len);
    return std::string(data);
}

void Parser::data(char* output, int len) {
    if(!this->stream) throw ParserException("Must open file for parsing first!");
    stream->read(output, len);
}

void Parser::pass(int len) {
    if(!this->stream) throw ParserException("Must open file for parsing first!");
    stream->seekg(len, ios::cur);
}

unsigned int Parser::getfilesize() {
    return this->filesize;
}

unsigned int Parser::getpos() {
    return this->stream->tellg();
}

