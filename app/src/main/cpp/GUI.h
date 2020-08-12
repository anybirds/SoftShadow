#ifndef GUI_H
#define GUI_H

#include <GLES3/gl3.h>

#include <ft2build.h>
#include FT_FREETYPE_H

class Shader;
class Material;

class GUI {
private:
    static Shader *uiVertexShader;
    static Shader *uiFragmentShader;
    static Material *uiMaterial;
    static Shader *textFragmentShader;
    static Material *textMaterial;

    static GLuint vao;
    static GLuint vbo;

    static FT_Library ft;

public:
    static void Init();

public:
    GUI() = delete;

    friend class Font;
    friend class GUIComponent;
    friend class Text;
};

#endif