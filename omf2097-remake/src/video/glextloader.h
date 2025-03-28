#ifndef GLEXTLOADER_H
#define GLEXTLOADER_H

/*
* Just a quick wrapper for glew.
* Just for separating it from the renderer.
*/

#ifdef _LINUX
#include <GL/glew.h>
#else
#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/wglew.h>
#endif

class GLExtLoader {
public:
    GLExtLoader();
};

#endif // GLEXTLOADER_H
