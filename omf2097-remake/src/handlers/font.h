#ifndef FONT_H
#define FONT_H

#include <string>

class Texture;
class Parser;
class Color;

class Font {
private:
    Parser *parser;
    int w,h;

public:
    Font(std::string fontfile, int w, int h);
    virtual ~Font();

    bool get_text_pix_size(int *w, int *h, std::string text);
    Texture* get_text_texture(std::string text, Color *c);
};

#endif // FONT_H
