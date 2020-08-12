#include <exception>

#include "GUI.h"
#include "Material.h"
#include "Shader.h"

#include "NDKHelper.h"

using namespace std;

Shader *GUI::uiVertexShader;
Shader *GUI::uiFragmentShader;
Material *GUI::uiMaterial;
Shader *GUI::textFragmentShader;
Material *GUI::textMaterial;
GLuint GUI::vao;
GLuint GUI::vbo;
FT_Library GUI::ft;

void GUI::Init() {
    if (FT_Init_FreeType(&ft)) {
        LOGI("Failed to init FreeType library");
        throw exception();
    }

    uiVertexShader = new Shader("ui_vert.glsl", GL_VERTEX_SHADER);
    uiFragmentShader = new Shader("ui_frag.glsl", GL_FRAGMENT_SHADER);
    uiMaterial = new Material(uiVertexShader, uiFragmentShader);
    uiMaterial->SetInteger("_UI_TEX", 0);
    textFragmentShader = new Shader("text_frag.glsl", GL_FRAGMENT_SHADER);
    textMaterial = new Material(uiVertexShader, textFragmentShader);
    textMaterial->SetInteger("_FONT_TEX", 0);

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)8);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}