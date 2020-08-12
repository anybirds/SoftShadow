#ifndef FONT_H
#define FONT_H

#include <glm/glm.hpp>

#include <string>
#include <map>

class Font {
private:
    struct Character {
        unsigned int textureId;
        glm::ivec2 size;
        glm::ivec2 bearing;
        unsigned int advance;
    };

private:
    std::map<char, Character> characters;

public:
    Font(const std::string &path);
    virtual ~Font();

    friend class Text;
};

#endif