#include "shaderprogram.h"

#include "tools/exception.h"
#include "tools/log.h"
#include "video/glextloader.h"
#include "video/glrenderer.h"

using namespace std;

ShaderProgram::ShaderProgram() {
    this->pid = glCreateProgram();
    this->in_use = false;
    this->linked = false;
    LOG("New program " << this->pid << " created!");
}

ShaderProgram::~ShaderProgram() {
    list<Shader*>::iterator it;
    for(it = this->shaders.begin(); it != this->shaders.end(); it++) {
        glDetachShader(this->pid, (*it)->sid);
        delete (*it);
    }
    this->use(false);
    glDeleteProgram(this->pid);
    LOG("Program " << this->pid << " deleted!");
}

void ShaderProgram::attach(Shader *shader) {
    this->shaders.push_back(shader);
    glAttachShader(this->pid, shader->sid);
    this->linked = false;
}

void ShaderProgram::detach(Shader *shader) {
    glDetachShader(this->pid, shader->sid);
    this->shaders.remove(shader);
    this->linked = false;
}

void ShaderProgram::use(bool u) {
    if(u && !in_use) {
        if(!linked) {
            glLinkProgram(this->pid);
            this->print_log();
            this->linked = true;
        }
        glUseProgram(this->pid);
        this->in_use = true;
    } else if(!u && in_use) {
        glUseProgram(0);
        this->in_use = false;
    }
#ifndef _OMFDEBUG
    int e = glGetError();
    if(e != GL_NO_ERROR) {
        THROW("Error while trying to use shader program " << this->pid << ": " << GLRenderer::get_error_string(e) << "!");
    }
#endif
}

void ShaderProgram::print_log() {
    int size = 0;
    glGetProgramiv(this->pid, GL_INFO_LOG_LENGTH, &size);

    if(size > 0) {
        char *log = new char[size];
        glGetProgramInfoLog(this->pid, size, 0, log);
        LOG(log);
        delete[] log;
    }
}

void ShaderProgram::set(std::string var, vec2i val) {
    float vi[2] = {val.x, val.y};
    unsigned int lc = glGetUniformLocation(this->pid, var.c_str());
    glUniform2fv(lc, 1, vi);
}

void ShaderProgram::set(std::string var, vec2f val) {
    float vf[2] = {val.x, val.y};
    unsigned int lc = glGetUniformLocation(this->pid, var.c_str());
    glUniform2fv(lc, 1, vf);
}

void ShaderProgram::set(std::string var, int val) {
    unsigned int lc = glGetUniformLocation(this->pid, var.c_str());
    glUniform1i(lc, val);
}
