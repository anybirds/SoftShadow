#ifndef SHADER_H
#define SHADER_H

#include <string>

#include <GLES3/gl3.h>

class Shader {
private:
    std::string path;
    GLenum type;

    GLuint id;

public:
    Shader(const std::string &path, GLenum type);
    virtual ~Shader();

    friend class Material;
};

#endif