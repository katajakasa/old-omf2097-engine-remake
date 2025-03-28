#include "glextloader.h"

#include "tools/log.h"

using namespace std;

GLExtLoader::GLExtLoader() {
    glewInit();
    LOG("GLExtLoader::GLExtLoader: GLEW Initialized.");
}
