#ifndef FONTFILE_H
#define FONTFILE_H

#include <string>

enum FontType {
    SMALL,
    LARGE
};

class FontFile {
protected:
    int _w,_h;
    char *data;
    int len;

    void load(FontType type);
    int getWidth(FontType type);

public:
    FontFile(std::string file, FontType type);
    ~FontFile();
    void save(std::string filename);

    int clen() { return this->_w; }
    int w() { return this->_w; }
    int h() { return this->_h; }
    char* get(char c);
    void set(char c, char* data);
};

#endif // FONTFILE_H
