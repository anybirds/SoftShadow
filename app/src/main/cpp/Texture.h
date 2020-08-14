#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

#include <GLES3/gl3.h>

class Texture {
private:
    GLuint id;

public:
    Texture(const std::string &path);
    virtual ~Texture();

    friend class Material;
    friend class GUIComponent;
    friend class Slider;
};
#endif