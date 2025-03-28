#ifndef PARSER_H
#define PARSER_H

#include <cinttypes>
#include <string>

class Parser {
protected:
    std::istream *stream;
    std::filebuf *fb;
    std::stringbuf *sb;
    unsigned int filesize;

public:
    Parser();
    Parser(std::string inputfile);
    Parser(const char* inputdata, int len);
    ~Parser();

    bool open(std::string inputfile);
    void close();

    int8_t byte();
    int16_t word();
    int32_t dword();
    uint8_t ubyte();
    uint16_t uword();
    uint32_t udword();
    char rchar();
    char peek_byte();
    uint32_t peek_udword();
    std::string str(int len);
    std::string cstr(int len);
    void data(char* output, int len);
    void pass(int len);
    unsigned int getfilesize();
    unsigned int getpos();
    void reset();
    void reset(unsigned int pos);

    bool isok();
};

#endif // PARSER_H
