#include "texture.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <string>
#include <cstring>
#include "tools/exception.h"
#include "tools/log.h"

Texture::Texture(int w, int h, int depth, char* data) {
    // Some checks
    if(w < 1) { THROW("Texture width must be over 0!"); }
    if(h < 1) { THROW("Texture hight must be over 0!"); }
    if(depth < 1) { THROW("Texture depth must be over 0!"); }

    // No errors; create buffer etc.
    this->w = w;
    this->h = h;
    this->depth = depth;
    this->data = new char[w*h*depth];
    if(data == 0) {
        memset(this->data, w*h*depth, 1);
    } else {
        memcpy(this->data, data, w*h*depth);
    }

    this->create();
}

Texture::~Texture() {
    this->remove();
    delete[] this->data;
}

void Texture::create() {
    glGenTextures(1, &this->id);
    glBindTexture(GL_TEXTURE_2D, this->id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->w, this->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, this->data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

#ifndef _OMFDEBUG
    if(glGetError() != GL_NO_ERROR) {
        THROW("Error while creating texture!");
    }
#endif
}

void Texture::remove() {
    glDeleteTextures(1, &this->id);
}

void Texture::validate() {
    if(glIsTexture(this->id) != GL_TRUE) {
        this->create();
    }
}

