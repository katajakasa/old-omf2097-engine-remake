#pragma once

#include <inttypes.h>
#include <fstream>
#include <sstream>
#include <string>

class Parser {
private:
    std::istream *stream;
    unsigned int filesize;

    std::filebuf fb;
    std::stringbuf sb;

public:
    Parser(std::string inputfile);
    Parser(char* inputdata, int len);
    ~Parser();

    int8_t byte();
    int16_t word();
    int32_t int3();
    int32_t dword();
    uint8_t ubyte();
    uint16_t uword();
    uint32_t uint3();
    uint32_t udword();
    char rchar();
    char peek_byte();
    uint32_t peek_udword();
    std::string str(int len);
    std::string cstr(int len);
    void data(char* output, int len);
    void pass(int len);
    void passndump(int len);
    unsigned int getfilesize() { return this->filesize; }
    unsigned int getpos() { return this->stream->tellg(); }
    void reset();
    void reset(unsigned int pos);

    bool isok() { return !this->stream->eof(); }
};
