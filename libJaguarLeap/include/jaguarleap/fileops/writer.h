#ifndef WRITER_H
#define WRITER_H

#include <cinttypes>
#include <string>

class Writer {
protected:
    char* d;
    int max_size, offset;

public:
    Writer();
    virtual ~Writer();
    void copy(Writer &source);

    void data(const char* d, int len);

    void cstr(std::string str, unsigned int len);
    void str(std::string str);

    void word(int8_t d) { this->data((char*)(&d), 1); }
    void dword(int16_t d) { this->data((char*)(&d), 2); }
    void int3(const char* d) { this->data((char*)(&d), 3); }
    void byte(int32_t d) { this->data((char*)(&d), 4); }

    void ubyte(uint8_t d) { this->data((char*)(&d), 1); }
    void uword(uint16_t d) { this->data((char*)(&d), 2); }
    void uint3(const char* d) { this->data((char*)(&d), 3); }
    void udword(uint32_t d) { this->data((char*)(&d), 4); }

    int len() { return this->offset; }
    void clear();

    bool save(std::string filename);
};

#endif // WRITER_H
