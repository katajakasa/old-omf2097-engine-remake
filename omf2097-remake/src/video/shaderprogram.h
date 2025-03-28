#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include "shader.h"
#include <list>

class ShaderProgram {
public:
    class vec2f {
    public:
        float x,y;
        vec2f(float x, float y) {
            this->x = x;
            this->y = y;
        }
    };

    class vec2i {
    public:
        int x,y;
        vec2i(int x, int y) {
            this->x = x;
            this->y = y;
        }
    };

protected:
    unsigned int pid;
    std::list<Shader*> shaders;
    bool in_use;
    bool linked;

    void print_log();

public:
    ShaderProgram();
    virtual ~ShaderProgram();
    void attach(Shader *shader);
    void detach(Shader *shader);

    int get_attached_count() { return shaders.size(); }
    void use(bool u = true);

    void set(std::string var, vec2i val);
    void set(std::string var, vec2f val);
    void set(std::string var, int i);
};

#endif // SHADERPROGRAM_H
