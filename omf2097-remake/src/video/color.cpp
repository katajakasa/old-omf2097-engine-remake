#include "color.h"

Color::Color(int r, int g, int b, int a) {
    this->_r = r/255;
    this->_g = g/255;
    this->_b = b/255;
    this->_a = a/255;
}

Color::Color(float r, float g, float b, float a) {
    this->_r = r;
    this->_g = g;
    this->_b = b;
    this->_a = a;
}

uint32_t Color::rgba() {
    uint32_t i = 0;
    i = this->r();
    i = i << 8 | this->g();
    i = i << 8 | this->b();
    i = i << 8 | this->a();
    return i;
}
