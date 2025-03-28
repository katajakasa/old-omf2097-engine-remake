#ifndef COLOR_H
#define COLOR_H

#include <inttypes.h>

class Color {
private:
    float _r,_g,_b,_a;

public:
    Color(int r, int g, int b, int a = 255);
    Color(float r, float g, float b, float a = 1.0);

    inline int r() { return this->_r*255; }
    inline float rf() { return this->_r; }
    inline int g() { return this->_g*255; }
    inline float gf() { return this->_g; }
    inline int b() { return this->_b*255; }
    inline float bf() { return this->_b; }
    inline int a() { return this->_a*255; }
    inline float af() { return this->_a; }

    uint32_t rgba();
};

#endif // COLOR_H
