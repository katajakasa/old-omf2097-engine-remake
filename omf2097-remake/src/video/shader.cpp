#include "shader.h"

#include "tools/exception.h"
#include "tools/log.h"
#include <fstream>

#include "video/glextloader.h"
#include "video/glrenderer.h"

using namespace std;

Shader::Shader(std::string file, Type t) {
    // open file
    ifstream in(file.c_str(), ifstream::in);
    if(!in.good()) {
        THROW("Error while opening shader file " << file);
    }
    in.seekg(0, ios::end);
    int size = in.tellg();
    in.seekg(0, ios::beg);

    // Read source
    const GLchar *sc = new char[size];
    in.read((char*)sc, size);

    // Create shader
    switch(t) {
        case VERTEX: this->sid = glCreateShader(GL_VERTEX_SHADER); break;
        case FRAGMENT: this->sid = glCreateShader(GL_FRAGMENT_SHADER); break;
        case GEOMETRY: this->sid = glCreateShader(GL_GEOMETRY_SHADER); break;
        default:
            THROW("Shader type not recognized!");
    }

    // Compile shader & print log
    glShaderSource(this->sid, 1, &sc, 0);
    glCompileShader(this->sid);
    this->print_log();
    delete[] sc;

#ifndef _OMFDEBUG
    int e = glGetError();
    if(e != GL_NO_ERROR) {
        THROW("Error while trying to compile shader " << file << ":" << GLRenderer::get_error_string(e) << "!");
    }
#endif

    LOG("Shader " << file << " compiled with id " << this->sid << ".");
}

Shader::~Shader() {
    glDeleteShader(this->sid);
    LOG("Shader " << this->sid << " deleted.");
}

void Shader::print_log() {
    int size = 0;
    glGetShaderiv(this->sid, GL_INFO_LOG_LENGTH, &size);

    if(size > 0) {
        char *log = new char[size];
        glGetShaderInfoLog(this->sid, size, 0, log);
        LOG(log);
        delete[] log;
    }
}
