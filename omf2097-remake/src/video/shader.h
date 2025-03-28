#ifndef SHADER_H
#define SHADER_H

#include <string>

class Shader {
public:
    enum Type {
        FRAGMENT = 0,
        VERTEX,
        GEOMETRY
    };

protected:
    void print_log();

public:
    unsigned int sid;

    Shader(std::string file, Type t);
    virtual ~Shader();
};

#endif // SHADER_H
