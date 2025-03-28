#pragma once

class Image;

class Texture {
private:
    void create();
    void remove();

public:
    unsigned int id;
    int w,h,depth;
    char* data;

    unsigned int size() { return w*h*depth; }
    void validate();

    Texture(int w, int h, int depth, char* data);
    ~Texture();
};
